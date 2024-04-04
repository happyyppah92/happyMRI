#include "myJobItem.h"

myJobItem::myJobItem()
{
	dateTime = QDateTime::currentDateTime();
	jobDescription = "none";
}

myJobItem::~myJobItem()
{
	curJobData.clear();
	curJobResult.clear();
}

//写3DJOb
void myJobItem::newJob(QString  jdescription, QDateTime dtime, dataMode dmode, ParaMRI pmri)//3D
{
	dateTime = dtime;
	jobDescription = jdescription;
	curJobData = dmode;
	curJobPara = pmri;
}

QString  myJobItem::getJobSeqdescrip() const
{
	return jobDescription;
}

QString  myJobItem::getNumImage() const
{
	return QString::number(curJobResult.size());
}


QDateTime myJobItem::getDateTime() const
{
	return dateTime;
}

void myJobItem::setJobSeqdescrip(QString str)
{
	this->jobDescription = str;
}

void myJobItem::setJobHead(QString head)
{
	jobHead = head;
}

QString myJobItem::getJobHead() const
{
	return jobHead;
}

