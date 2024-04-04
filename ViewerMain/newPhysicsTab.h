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
	int updateData(dataMode data);//��ֵ����������
	void updateCurves();//��ҪдROIList
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

	//�ֲ�������Ϊ�˼��û��д�ɺ����ľֲ�����
	AnnoItemGroup myAnnoTL;//��ǰ��ǩ
	AnnoItemGroup myAnnoBL;
	QPoint xyCrossPos1;//��ߵ�ʮ����
	QPoint xyCrossPos2;// ��ߵ�ʮ����

	dataMode curData;//��ǰ����
	DataImage currawImage;//��ǰ��ʾ�� ��ߵ�ͼ��
	DataImage curMRIimage;//��ǰ��ʾ�� �ұߵ�ͼ��
	int curImageIndex = 0;

	bool isROImode = false;
	bool isAutoWP = false;
	bool isGetWPWin = false;

	int TEvalue;
	int TRvalue;
	QPair<double, double> myWin;
signals:
	void updateStatus(QString msg, msgClassEnum msgclass);//����״̬

public slots:
	void on_addROIBtnClicked();
	void on_TESliderChanged(int value);
	void on_TRSliderChanged(int value);
	void on_autoWPcheckStateChanged(int value);
	void on_legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
	void on_copyImage();
};
