#include "myFileopen.h"
#include <QFileInfo>
#include <QtDebug>
#include <QDataStream>
#include <QVector>
#ifdef DEBUG
#include <QTime>
QTime startTime = QTime::currentTime();
QTime stopTime = QTime::currentTime();
int elapsed = startTime.msecsTo(stopTime);
#endif // DEBUG



//根据文件类型，进行文件读取，将数据给 dataMode
//不进行任何界面操作
myFileOpen::myFileOpen()
{
}

myFileOpen::~myFileOpen()
{
	currentData.clear();
}
//打开原始数据,对外的接口
//根据数据的后缀选择对应的函数，匹配不同的数据源，保存数据到dataMode中
proStatuEnum myFileOpen::rawPatientFileOpen(QFile &file,dataMode &datamode)
{
    QFileInfo fileinfo=QFileInfo(file);
    qDebug() << fileinfo.suffix();
	proStatuEnum s;
    switch (SUFFIXLIST.indexOf(fileinfo.suffix()))//字符串转索引
    {
    //读MRI数据
    case 0://MRI后缀
        qDebug() << "rawPatientFileOpen_MRIdata";
        s=rawPatientFileOpen_MRIdata(file);
        if(s==proStatuEnum::OK){
            currentData.dataFrom=dataFromEnum::mrisimWin;
            currentData.fileName=file.fileName();
            datamode= currentData;
            return proStatuEnum::OK;
        }
        else {
            return proStatuEnum::ERR;
        }
        break;

        //读MRILab的数据
    case 1://data后缀
        qDebug() << "rawPatientFileOpen_Otherdata";
        s=rawPatientFileOpen_Otherdata(file);
        if (s==proStatuEnum::OK){
            currentData.dataFrom=dataFromEnum::MRIlab;
            currentData.fileName=file.fileName();
            datamode =currentData;
            return proStatuEnum::OK;
        }
        break;
    }
    return proStatuEnum::ERR;
}

//读MRI数据
proStatuEnum  myFileOpen::rawPatientFileOpen_MRIdata(QFile &file)
{
    //打开数据
    QDataStream inStream;//数据流
    QByteArray array;//数据流 变量
    QVector<double> rawData;//存放原始的二进制图像数据

    inStream.setDevice(&file);
    inStream.setVersion(QDataStream::Qt_5_7);
    array.resize(8);
    inStream.readRawData(array.data(),8);
    currentData.dataFrom=dataFromEnum::MRIlab;
    // 1
    if (qstrcmp(array.data(),";;mbfmat")==0)//一般的数据，看数据的组织形式
    {
        inStream.readRawData(array.data(),8);
        //大小
        array.resize(4);
        inStream.readRawData(array.data(),4);
        currentData.widthImage= *((long*)array.data());//后读
        inStream.readRawData(array.data(),4);
        currentData.heightImage=  *((long*)array.data());//先读

        //读所有的数据,先w后h
        long  byteSize=currentData.widthImage*currentData.heightImage;

        if(byteSize< MAXDATASIZE)
        {
            array.resize(1);
            long i=0;
            for(i;i<byteSize;i++)
            {
                inStream.readRawData(array.data(),8);//double
                rawData.push_back(*((double*)array.data()));
            }
        }
    }
    else{//其他数据类型
        file.reset();
        inStream.readRawData(array.data(),4);
        currentData.widthImage= *((long*)array.data());
        inStream.readRawData(array.data(),4);
        currentData.heightImage=  *((long*)array.data());
        qDebug()<<"MRI OtherFile:";

        //读所有的数据
        unsigned long  byteSize=currentData.heightImage*currentData.widthImage;
        if(byteSize< MAXDATASIZE)
        {
            array.resize(1);
            for(unsigned long i=0;i<byteSize;i++)
            {
                inStream.readRawData(array.data(),4);//float
                rawData.push_back(*((float*)array.data()));
            }
        }
    }

    //读最后面的说明
    array.resize(100);
    inStream.readRawData(array.data(),100);
    currentData.fileDescription=array;
    file.close();

    //根据文件描述分离数据，全是2D的数据
    if(currentData.fileDescription.contains("MRI PD,T1,T2,DTI DATA")){//DTI数据，多幅图
        int numImage=currentData.widthImage/currentData.heightImage;//
        qDebug()<<numImage;
        currentData.widthImage=currentData.widthImage/ numImage;
        //所有的数据放在PD
        int iNumImage=0;
        QVector<double> temp(currentData.widthImage*currentData.heightImage);
        for(int iW=0;iW<currentData.widthImage;++iW){
            for(int iH=0;iH<currentData.heightImage;++iH){
                int index=iNumImage*(currentData.widthImage*currentData.heightImage)+iW*currentData.heightImage+iH;
                int indextemp= iH*currentData.widthImage+iW;//目标的组织形式，先w后H
                temp.replace(indextemp,rawData.at(index)/1000.0);
            }
        }
        currentData.PDmap.append(temp);

        iNumImage=1;
        for(int iW=0;iW<currentData.widthImage;++iW){
            for(int iH=0;iH<currentData.heightImage;++iH){
                int index=iNumImage*(currentData.widthImage*currentData.heightImage)+iW*currentData.heightImage+iH;
                int indextemp= iH*currentData.widthImage+iW;//目标的组织形式，先w后H
                temp.replace(indextemp,rawData.at(index)/1000.0);
            }
        }
        currentData.T1map.append(temp);

        iNumImage=2;
        for(int iW=0;iW<currentData.widthImage;++iW){
            for(int iH=0;iH<currentData.heightImage;++iH){
                int index=iNumImage*(currentData.widthImage*currentData.heightImage)+iW*currentData.heightImage+iH;
                int indextemp= iH*currentData.widthImage+iW;//目标的组织形式，先w后H
                temp.replace(indextemp,rawData.at(index)/1000.0);
            }
        }
        currentData.T2map.append(temp);

        currentData.pixel2mm=0;
        currentData.dataType=dataTypeEnum::MAP2D;
        return proStatuEnum::OK;

    }else if(currentData.fileDescription.contains("MRI PD,T1,T2 DATA")){//一般数据
        int byteSizeT1;
        byteSizeT1=currentData.widthImage/3*currentData.heightImage;//每个数据占的大小
        //分离数据
        QVector<double> temp1PD,temp1T1,temp1T2;
        int i;
        for (i=0;i<byteSizeT1;i++)
        {
            temp1PD.push_back(rawData.at(i)/1000.0);
        }

        for (i=0;i<byteSizeT1;i++)
        {
            temp1T1.push_back(rawData.at(byteSizeT1+i)/1000.0);
        }

        for (i=0;i<byteSizeT1;i++)
        {
            temp1T2.push_back(rawData.at(byteSizeT1*2+i)/1000.0);
        }

        currentData.widthImage=currentData.widthImage/3;//这种数据结构下有3种数据类型

        //这个数据需要翻转
        QVector<double> temp2PD,temp2T1,temp2T2;
        int tempH,tempW;
        temp2PD=temp1PD;
        temp2T1=temp1T1;
        temp2T2=temp1T2;
        tempW=currentData.widthImage;
        tempH=currentData.heightImage;

        for(int iw=0;iw<currentData.widthImage;++iw){
            for(int ih=0;ih<currentData.heightImage;++ih){
                int index=iw*currentData.heightImage+ih;
                int index2=ih*currentData.widthImage+iw;
                temp2PD.replace(index2,temp1PD.at(index));
                temp2T1.replace(index2,temp1T1.at(index));
                temp2T2.replace(index2,temp1T2.at(index));
            }
        }
        currentData.heightImage=tempW;
        currentData.widthImage=tempH;
        currentData.PDmap.append(temp2PD);
        currentData.T1map.append(temp2T1);
        currentData.T2map.append(temp2T2);
        currentData.pixel2mm=0;
        currentData.dataType=dataTypeEnum::MAP2D;

		temp1PD.clear();
		temp1T1.clear();
		temp1T2.clear();

		temp2PD.clear();
		temp2T1.clear();
		temp2T2.clear();
        return proStatuEnum::OK;
    }else if(currentData.fileDescription.contains("fmri")){ //fMRI数据，图像时间序列
        //获取序列长度
        QStringList strList=currentData.fileDescription.split('\n');
        int numImage=strList.at(1).toUInt();
        currentData.heightImage=currentData.heightImage/ numImage;
        //所有的数据放在PD
        for(int iNumImage=0;iNumImage<numImage;++iNumImage){
            QVector<double> temp2PD(currentData.widthImage*currentData.heightImage);
            for(int iW=0;iW<currentData.widthImage;++iW){
                for(int iH=0;iH<currentData.heightImage;++iH){
                    int index=iW*(numImage*currentData.heightImage)+iNumImage*currentData.heightImage+iH;
                    int indextemp= iH*currentData.widthImage+iW;//目标的组织形式，先w后H
                    temp2PD.replace(indextemp,rawData.at(index));
                }
            }
            currentData.PDmap.append(temp2PD);
        }
        currentData.pixel2mm=0;
        currentData.dataType=dataTypeEnum::ImageTime;//纯粹的图像，不能运算
        return proStatuEnum::OK;
    }else {//单纯的图像
        int byteSizeT1;
        byteSizeT1=currentData.widthImage*currentData.heightImage;//每个数据占的大小
        //分离数据
        QVector<double> temp1PD;
        int i;
        for (i=0;i<byteSizeT1;i++)
        {
            temp1PD.push_back(rawData.at(i)/1000.0);
        }

        //这个数据需要翻转
        QVector<double> temp2PD;
        int tempH,tempW;
        temp2PD=temp1PD;
        tempW=currentData.widthImage;
        tempH=currentData.heightImage;

        for(int iw=0;iw<currentData.widthImage;++iw){
            for(int ih=0;ih<currentData.heightImage;++ih){
                int index=iw*currentData.heightImage+ih;
                int index2=ih*currentData.widthImage+iw;
                temp2PD.replace(index2,temp1PD.at(index));
            }
        }
        currentData.heightImage=tempW;
        currentData.widthImage=tempH;
        currentData.PDmap.append(temp2PD);
        currentData.pixel2mm=0;
        currentData.dataType=dataTypeEnum::Image2D;

		temp1PD.clear();
		temp2PD.clear();
        return proStatuEnum::OK;
    }
	array.clear();
	rawData.clear();
    return proStatuEnum::ERR;
}

//读其他数据
proStatuEnum  myFileOpen::rawPatientFileOpen_Otherdata(QFile &file)
{
#ifdef DEBUG
	startTime = QTime::currentTime();
#endif // DEBUG

    //打开数据
    QDataStream inStream;//数据流
    QByteArray array;//数据流 变量
    QVector<double> rawData;//存放原始的二进制图像数据，仅为小一点的数据
	char *tempChar;
	double *tempdouble; //大块的数据

    int numImage=1,numMode=1;

    inStream.setDevice(&file);
    inStream.setVersion(QDataStream::Qt_5_7);
    array.resize(8);
    inStream.readRawData(array.data(),8);
    currentData.dataFrom=dataFromEnum::MRIlab;
    // 1
    if (qstrcmp(array.data(),";mridata")==0)//一般的数据，看数据的组织形式
    {
        array.resize(4);
        inStream.readRawData(array.data(),4);//long 空
        inStream.readRawData(array.data(),4);//版本
        inStream.readRawData(array.data(),4);//版本
        currentData.widthImage= *((long*)array.data());//后读
        inStream.readRawData(array.data(),4);
        currentData.heightImage=  *((long*)array.data());//先读

        inStream.readRawData(array.data(),4);
        numImage= *((long*)array.data());//后读
        inStream.readRawData(array.data(),4);
        numMode=  *((long*)array.data());//先读

        inStream.readRawData(array.data(),4);//空

        //读所有的数据,先w后h
        long  byteSize=currentData.widthImage*currentData.heightImage*numImage*numMode;
		tempChar = new char[byteSize * 8];
		inStream.readRawData(tempChar, byteSize * 8);//double
		tempdouble = reinterpret_cast<double*>(tempChar);
    }
    //读最后面的说明
    array.resize(100);
    inStream.readRawData(array.data(),100);
    currentData.fileDescription=array;
    file.close();
#ifdef DEBUG
	stopTime = QTime::currentTime();
	elapsed = startTime.msecsTo(stopTime);
	qDebug() << "read File =" << elapsed << "ms";
#endif // DEBUG

    //根据文件描述分离数据，全是2D的数据
    if(currentData.fileDescription.contains("PD;T1;T2")){
        int byteSizeT1=currentData.widthImage*currentData.heightImage;//每个数据占的大小
        int byteSizeBlock=byteSizeT1*numImage;

        //所有的数据放在PD
        int iblock=0;
        for(int iN=0;iN<numImage;++iN)
		{
            QVector<double> temp1PD(currentData.widthImage*currentData.heightImage);
            for(int iH=0;iH<currentData.heightImage;++iH){
                for(int iW=0;iW<currentData.widthImage;++iW){
                    int index=iblock*byteSizeBlock+ iN*byteSizeT1+iH*currentData.widthImage+iW;
                    int indextemp= iH*currentData.widthImage+iW;//目标的组织形式，先w后H
                    temp1PD.replace(indextemp, *(tempdouble+index));
                }
            }
            currentData.PDmap.append(temp1PD);
			temp1PD.clear();
        }
        iblock=1;
        for(int iN=0;iN<numImage;++iN){
            QVector<double> temp1T1(currentData.widthImage*currentData.heightImage);
            for(int iH=0;iH<currentData.heightImage;++iH){
                for(int iW=0;iW<currentData.widthImage;++iW){
                    int index=iblock*byteSizeBlock+ iN*byteSizeT1+iH*currentData.widthImage+iW;
                    int indextemp= iH*currentData.widthImage+iW;//目标的组织形式，先w后H
                    temp1T1.replace(indextemp,*(tempdouble + index));
                }
            }
            currentData.T1map.append(temp1T1);
			temp1T1.clear();
        }
        iblock=2;
        for(int iN=0;iN<numImage;++iN){
            QVector<double> temp1T2(currentData.widthImage*currentData.heightImage);
            for(int iH=0;iH<currentData.heightImage;++iH){
                for(int iW=0;iW<currentData.widthImage;++iW){
                    int index=iblock*byteSizeBlock+ iN*byteSizeT1+iH*currentData.widthImage+iW;
                    int indextemp= iH*currentData.widthImage+iW;//目标的组织形式，先w后H
                    temp1T2.replace(indextemp,*(tempdouble + index));
                }
            }
            currentData.T2map.append(temp1T2);
			temp1T2.clear();
        }
        currentData.pixel2mm=0;
        if(numImage>=2){
            currentData.dataType=dataTypeEnum::MAP3D;
        }else {
            currentData.dataType=dataTypeEnum::MAP2D;
        }
#ifdef DEBUG
		stopTime = QTime::currentTime();
	    elapsed = startTime.msecsTo(stopTime);
		qDebug() << "分离文件 =" << elapsed << "ms";
#endif // DEBUG
		return proStatuEnum::OK;
    }
	array.clear();
	rawData.clear();
	delete tempChar;
	delete tempdouble;
    return proStatuEnum::ERR;
}
