#pragma once

#include <QObject>
#include <QVector>
#include "dataMode.h"
#include "./other/qcustomplot.h"

class mySECurve : public QWidget
{
	Q_OBJECT

public:
	mySECurve(QWidget *parent=0);
	~mySECurve();
	void updateCurve(QList<QPair<QVector<double>, QVector<double>>> curveList, QList<ROImode> ROIList);
	void clearCurves();
	void setAutoXY();
	QCustomPlot *curvePlot;
private:
	QVector<double> curdata;
	double xmaxvalue=0;
	double ymaxvalue=0;
private slots:
};