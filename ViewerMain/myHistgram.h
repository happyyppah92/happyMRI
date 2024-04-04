#pragma once

#include <QObject>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVector>
#include "dataMode.h"
#include "./other/qcustomplot.h"

class myHistgram : public QWidget
{
	Q_OBJECT

public:
	myHistgram(QWidget *parent=0);
	~myHistgram();
	void updateHistgarm(QImage image);
	void setAutoY();
private:
	void calhistogram(QImage image, QPair<QVector<double>, QVector<double>> &hdata);
	void refreshData(QPair<QVector<double>, QVector<double>> &hdata);
	QCPGraph *cb;
	QCustomPlot *histplot;
	QPushButton *autoY;
	QVector<double> curdata;
private slots:
	void on_autoYClicked();

};
