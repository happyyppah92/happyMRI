#pragma once

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QTableView>
#include <QTabWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <qslider.h>

#include "mySECurve.h"
#include "myKspaceView.h"
#include "./ViewerMain/mytableview.h"

#include "myRoitableview.h"
#include "physicpara.h"
#include "dataMode.h"
#include "global.h"

class NewPhysicsTab : public QWidget
{
	Q_OBJECT

public:
	NewPhysicsTab(QWidget *parent);
	~NewPhysicsTab();
	QPushButton *addRoiBtn;
	physicpara *phyPara;
	int updateData(dataMode data);//将值传过来即可
	void updateCurves();//先要写ROIList
	void updateMRIdataView();
	void closePatient();
	QList<ROImode> ROIList;

public slots:
	void on_updateCrossLines(QPointF pos);
	void on_roiSelect(QPointF pos);
	void on_jobDelectAll();
	void on_jobDelectItem();

private:
	void updateImages();
	void updateCrossLine1();
	void updateCrossLine2();


	myRoitableview *roiTable;
	mykspaceview *rawDataView;
	mykspaceview *MRIdataView;

	mySECurve *SECurve;

	//局部变量，为了简便没有写成函数的局部变量
	AnnoItemGroup myAnnoTL;//当前标签
	AnnoItemGroup myAnnoBL;
	QPoint xyCrossPos1;//左边的十字线
	QPoint xyCrossPos2;// 左边的十字线

	dataMode curData;//当前数据
	DataImage currawImage;//当前显示的 左边的图像
	DataImage curMRIimage;//当前显示的 右边的图像
	int curImageIndex = 0;

	bool isROImode = false;
	bool isAutoWP = false;
	bool isGetWPWin = false;

	int TEvalue;
	int TRvalue;
	QPair<double, double> myWin;
signals:
	void updateStatus(QString msg, msgClassEnum msgclass);//发射状态

public slots:
	void on_addROIBtnClicked();
	void on_TESliderChanged(int value);
	void on_TRSliderChanged(int value);
	void on_autoWPcheckStateChanged(int value);
	void on_legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
	void on_copyImage();
};
