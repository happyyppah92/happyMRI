#ifndef MYFILEOPEN_H
#define MYFILEOPEN_H
#include "dataMode.h"
#include "global.h"
#include <QFile>
//用于打开不同类型的文件，存数据到datamode中
class myFileOpen
{
public:
    myFileOpen();
	~myFileOpen();
	proStatuEnum rawPatientFileOpen(QFile &file,dataMode &datamode);//打开病人，对外的接口,为保证一致性传引用

private:
	proStatuEnum  rawPatientFileOpen_MRIdata(QFile &file);
	proStatuEnum  rawPatientFileOpen_Otherdata(QFile &file);
    dataMode currentData;//当前的数据
};
#endif // MYFILEOPEN_H
