#include "myHistgram.h"

myHistgram::myHistgram(QWidget *parent)
{
	Q_UNUSED(parent)
	histplot = new QCustomPlot;
	histplot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
	histplot->axisRect()->setRangeZoom(Qt::Orientation::Vertical);
	histplot->axisRect()->setRangeDrag(Qt::Orientation::Vertical);
	histplot->xAxis->setRange(QCPRange(0, 255));
	histplot->xAxis->setNumberFormat("g");
	histplot->yAxis->setTickLabels(false);
	histplot->setBackground(QBrush("#000033"));
	histplot->xAxis->setTickLabelColor(Qt::white);
	histplot->yAxis->setTickLabelColor(Qt::white);
	histplot->xAxis->setTickPen(QPen(Qt::white));
	histplot->yAxis->setTickPen(QPen(Qt::white));
	histplot->xAxis->setBasePen(QPen(Qt::white, 1, Qt::PenStyle::SolidLine));
	histplot->yAxis->setBasePen(QPen(Qt::white, 1, Qt::PenStyle::SolidLine));
	histplot->xAxis->setSubTicks(false);
	histplot->yAxis->setSubTicks(false);
	histplot->xAxis->grid()->setVisible(false);
	histplot->yAxis->grid()->setVisible(false);
	cb = new QCPGraph(histplot->xAxis, histplot->yAxis);
	cb->setPen(QPen(Qt::green));

	autoY = new QPushButton;
	autoY->setText("Auto");

	QHBoxLayout *hlay = new QHBoxLayout;
	hlay->addWidget(autoY);
	hlay->addWidget(histplot);
	hlay->setStretchFactor(autoY, 1);
	hlay->setStretchFactor(histplot, 9);
	setLayout(hlay);

	connect(autoY, SIGNAL(clicked()), this, SLOT(on_autoYClicked()));
}

myHistgram::~myHistgram()
{

}

void myHistgram::on_autoYClicked()
{
	setAutoY();
}
void myHistgram::updateHistgarm(QImage image)
{
	QPair<QVector<double>, QVector<double>> hdata;
	calhistogram(image, hdata);
	refreshData(hdata);
	hdata.first.clear();
	hdata.second.clear();
}

void myHistgram::refreshData(QPair<QVector<double>, QVector<double>> &data)
{
	curdata = data.second;
	cb->setData(data.first, data.second);;
	histplot->replot();
}

void myHistgram::setAutoY()
{
	double maxvalue = *std::max_element(curdata.constBegin() + 20, curdata.constEnd());
	histplot->yAxis->setRangeLower(0);
	histplot->yAxis->setRangeUpper(maxvalue);
	histplot->replot();
}
void myHistgram::calhistogram(QImage image, QPair<QVector<double>, QVector<double>> &hdata)
{
	int nWidth = image.width();
	int nHeight = image.height();

	QVector<double> vecX(256, 0);
	QVector<double> vecY(256, 0);   //init Y data with 0;
	if (nWidth && nHeight)
	{
		int i = 0;
		while (256 != i) {
			vecX.replace(i, i);
			++i;
		}

		for (int j = 0; j < nHeight; j++)
		{
			for (int k = 0; k < nWidth; k++)
			{
				int nIndex = int(image.bits()[j*nWidth + k]);
				vecY[nIndex] = vecY.at(nIndex) + 1;
			}
		}
	}
	hdata.first = vecX;
	hdata.second = vecY;

	vecX.clear();
	vecY.clear();
}

