//数据模型
#include "dataMode.h"
#include <QVector>
#include <QFile>

dataMode::dataMode()
{
    heightImage=0;
    widthImage=0;
    fileName="";
    fileDescription="";
	pixel2mm = 0.0;
	dataFrom = dataFromEnum::Other;
	dataType = dataTypeEnum::MAP2D;
}

dataMode::~dataMode()
{
	clear();

}
dataMode dataMode::copyValue()
{
    dataMode data;
    data.heightImage=this->heightImage;
    data.widthImage=this->widthImage;

    data.PDmap = this->PDmap;
    data.T1map =this->T1map;
    data.T2map =this->T2map;

    data.fileName =this->fileName;
    data.fileDescription=this->fileDescription;
    data.pixel2mm=this->pixel2mm;
    data.dataFrom =this->dataFrom;
    data.dataType =this->dataType;
    return data;
}

void dataMode::clear()
{
    PDmap.clear();
    T1map.clear();
    T2map.clear();
	T2Startmap.clear();
}




