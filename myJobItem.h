//一个job 包括job 数据 参数 结果
#ifndef MYJOBITEM_H
#define MYJOBITEM_H

#include "global.h"
#include "dataMode.h"
#include "./ViewerMain/paraControl.h"
#include <QString>
#include <QList>
#include <QImage>
#include <QDateTime>
#include <QTime>
#include <QVector>

//Job的数据结构，仅用于显示在JOB栏和图像
//dataMode的数据结构是用于计算的
class myJobItem
{
public:
    myJobItem();
	~myJobItem();
	dataMode curJobData;//数据模型
    ParaMRI curJobPara;//参数模型
	QList<QVector <double>> curJobResult;//结果

    void newJob(QString  jdescription, 
		QDateTime dtime, 
		dataMode dmode, 
		ParaMRI pmri);//3D
	void setJobHead(QString head);
	void setJobSeqdescrip(QString str);

	QString  getJobSeqdescrip() const;
	QString  getNumImage() const;
	QDateTime getDateTime() const;
	QString getJobHead() const;
private:
	QString  jobDescription;//文件描述
	QString jobHead;
	QDateTime dateTime;//job时间
};

#endif // MYJOBITEM_H
