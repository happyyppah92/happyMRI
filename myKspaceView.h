#pragma once

#include <QGraphicsView>
#include <QGridLayout>
#include "myGraphyView.h"
#include "./other/qcustomplot.h"
//在myGraphyView的基础上加了2个plot
//为了简便只负责维护2个plot
class mykspaceview : public QWidget
{
	Q_OBJECT

public:
	mykspaceview(QWidget *parent=0);
	~mykspaceview();
	void updataXYPlot(QVector<double>& data,
		int imageWidth, int imageHeight, int x, int y);	
	myGraphyView *mygv;//中间的图像窗口
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
