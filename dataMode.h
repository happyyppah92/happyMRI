//与界面无关
#ifndef DATAMODE_H
#define DATAMODE_H
#pragma once
#include "global.h"
#include <QVector>
#include <QFile>
#include <QColor>
#include <qrect.h>

//数据模型
//读到的数据，以数据模型的方式进行保存，数据以数据模型的方式进行传递
//统一转化成double 进行存储，在进行计算的时候输入dataMode，输出 图像
class dataMode
{
public:
    dataMode();
	~dataMode();
    int heightImage; //图像的高度
    int widthImage; //图像的宽度
    QList<QVector<double>> PDmap;//3D
    QList<QVector<double>> T1map;//3D
    QList<QVector<double>> T2map;//3D
	QList<QVector<double>> T2Startmap;//支持3D
    QString  fileName;//文件名
    QString  fileDescription;//文件描述,任意的话
    double pixel2mm;//像素和实际尺寸

    dataFromEnum dataFrom;//mrisimWin还是MRIlab
    dataTypeEnum dataType;//多少D的数据

    //参数,可以补充更多的参数
    dataMode copyValue();
    void clear();
};

//参数模型
//磁共振的一些参数，在这里补充更多的参数
typedef struct ParaMRI
{
	seqTypeEnum Seqtype = seqTypeEnum::SE;
	QString description = "";
	//参数,可以补充更多的参数
	double TE = 0.0;//单位ms
	double TR = 0.0;//单位ms
	double TI = 0.0;//参数
	double Flipangle = 90;//单位度
	double Average = 1;
	reconstructionEnum Reconstruction = reconstructionEnum::real;
	double isFastSE = false;
	int ETL = 1;
	int Bandwidth = 32;
	double FOVread = 100;
	double FOVphase = 100;
	double Slicethickness = 10;
	int Freresolution = 256;
	int slices = 16;
	phaseDirEnum phaseDir = phaseDirEnum::A2P;
}ParaMRI;

//一张图
typedef struct DataImage
{
	~DataImage()
	{
		resultReal.clear();
	}
	QList<QVector<double>> resultReal;//实部,虚部
	int width;
	int height;
}DataImage;

//参数模型
//磁共振的一些参数，在这里补充更多的参数
typedef struct ParaPro
{
	~ParaPro()
	{
		pixchange.clear();
	}
	bool isSmooth;
	int smoothtype;
	int smoothsize;

	bool isWindow;
	int windowtype;
	int windowsize;

	bool isPF;
	int PFtype;
	int PFPercent;

	bool isUS;
	int UStype;
	int USPercent;

	bool isPixchange;
	QVector<QPair<QPoint, double>> pixchange;
}ParaPro;

//ROI数据
typedef struct ROImode
{
	int  ROIorder;
	bool isChecked;
	QString disp;
	QRectF xyRect;
	QColor ROIColor;

	int T1;
	int T2;
	int PD;//ms
	int T2Start;
}ROImode;

#endif // DATAMODE_H
