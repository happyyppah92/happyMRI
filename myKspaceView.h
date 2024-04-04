#pragma once

#include <QGraphicsView>
#include <QGridLayout>
#include "myGraphyView.h"
#include "./other/qcustomplot.h"
//��myGraphyView�Ļ����ϼ���2��plot
//Ϊ�˼��ֻ����ά��2��plot
class mykspaceview : public QWidget
{
	Q_OBJECT

public:
	mykspaceview(QWidget *parent=0);
	~mykspaceview();
	void updataXYPlot(QVector<double>& data,
		int imageWidth, int imageHeight, int x, int y);	
	myGraphyView *mygv;//�м��ͼ�񴰿�
	QCustomPlot *xplot;
	QCPGraph *xplotGraph;
	QCustomPlot *yplot;
	QCPGraph *yplotGraph;
	QGridLayout *lay;

private:
	void updateXYPlot();
	QVector<double> curdata;
	int curwidth;
	int curheigt;
	int curXpos;
	int curYpos;
};
