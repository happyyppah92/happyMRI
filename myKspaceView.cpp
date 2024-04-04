#include "myKspaceView.h"

mykspaceview::mykspaceview(QWidget *parent)
{
	Q_UNUSED(parent)

	mygv = new myGraphyView(0,false);//(QWidget *parent, bool isLinkAllEnable)
	mygv->enableCross(true);
	mygv->setAlignment(Qt::AlignHCenter);

	xplot = new QCustomPlot;
	xplot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
	xplot->axisRect()->setRangeZoom(Qt::Orientation::Vertical);
	xplot->xAxis->setRange(QCPRange(0, 255));
	xplot->yAxis->setRange(QCPRange(0, 255));
	xplot->xAxis->setNumberFormat("g");
	xplot->yAxis->setNumberFormat("g");
	xplot->yAxis->setPadding(0);
	xplot->setBackground(QBrush("#000033"));
	xplot->xAxis->setVisible(false);
	xplot->yAxis->setVisible(false);
	xplotGraph = new QCPGraph(xplot->xAxis, xplot->yAxis);
	xplotGraph->setPen(QPen(Qt::green));

	yplot = new QCustomPlot;
	yplot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
	yplot->axisRect()->setRangeZoom(Qt::Orientation::Horizontal);
	yplot->yAxis->setRange(QCPRange(0, 255));
	yplot->xAxis->setRange(QCPRange(0, 255));
	yplot->yAxis->setRangeReversed(true);
	yplot->xAxis->setNumberFormat("g");
	yplot->yAxis->setNumberFormat("g");
	yplot->xAxis->setPadding(0);
	yplot->setBackground(QBrush("#000033"));
	yplot->xAxis->setVisible(false);
	yplot->yAxis->setVisible(false);
	yplotGraph = new QCPGraph(yplot->yAxis, yplot->xAxis);
	yplotGraph->setPen(QPen(Qt::green));

	lay = new QGridLayout;
	lay->addWidget(yplot, 0, 0, 8, 1);
	lay->addWidget(mygv,0, 1, 8, 7);
	lay->addWidget(xplot, 8, 1, 1, 7);

	for (int itemp = 1; itemp < 8;++itemp)
	{
		lay->setColumnStretch(itemp, 1);
	}
	for (int itemp = 1; itemp < 8; ++itemp)
	{
		lay->setRowStretch(itemp, 1);
	}
	setLayout(lay);
}

mykspaceview::~mykspaceview()
{
}
void mykspaceview::updateXYPlot()
{
	QVector<double> Xvec;
	QVector<double> Yvec;
	int ih = curYpos;//横线
	int iw = curXpos;
	iw = iw < 0 ? 0 : iw;
	iw = iw > curwidth - 1 ? curwidth - 1 : iw;
	ih = ih < 0 ? 0 : ih;
	ih = ih > curheigt - 1 ? curheigt - 1 : ih;

	// 横向曲线,数据是先横
	for (int iw = 0; iw < curwidth; ++iw)
	{
		Xvec.append(iw);
		Yvec.append(curdata[ih*curwidth + iw]);
	}
	xplotGraph->setData(Xvec, Yvec);
	double Imagemin = *std::min_element(Yvec.constBegin(), Yvec.constEnd());
	double Imagemax = *std::max_element(Yvec.constBegin(), Yvec.constEnd());
	xplot->xAxis->setRange(0, Yvec.size());
	xplot->yAxis->setRange(-0.1, Imagemax*1.1);
	xplot->replot();

	// 高度
	Yvec.clear();
	Xvec.clear();
	for (int ih = 0; ih < curheigt; ++ih)
	{
		Xvec.append(ih);
		Yvec.append(curdata[ih*curwidth + iw]);
	}
	yplotGraph->setData(Xvec, Yvec);;
	Imagemin = *std::min_element(Yvec.constBegin(), Yvec.constEnd());
	Imagemax = *std::max_element(Yvec.constBegin(), Yvec.constEnd());
	yplot->xAxis->setRange(-0.1, Imagemax*1.1);
	yplot->yAxis->setRange(0, Yvec.size());
	yplot->replot();
	mygv->updateMyCrossLine(curXpos,curYpos);

	Xvec.clear();
	Yvec.clear();
}
void mykspaceview::updataXYPlot(QVector<double>& data, 
	int imageWidth,int imageHeight, int x, int y)
{
	curdata = data;
	curwidth = imageWidth;
	curheigt = imageHeight;
	curXpos = x;
	curYpos = y;
	curwidth = imageWidth;
	curheigt = imageHeight;

	updateXYPlot();
}