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

#include "myHistgram.h"
#include "myKspaceView.h"
#include "mytableview.h"
#include "kspacepara.h"
#include "myProcess.h"
#include "global.h"

class NewKspaceTab : public QWidget
{
	Q_OBJECT

public:
	NewKspaceTab(QWidget *parent = 0);
	~NewKspaceTab();
	QPushButton *newLoaddataBtn;
	QList<myJobItem> myJobList;
	void updateData();
	void closePatient();

public slots:
	void on_JobSelect(const QModelIndex index);

	//参数界面的槽
	void on_smoothClick(int);
	void on_windowClick(int);
	void on_PFClick(int);
	void on_undersampleClick(int);

	//图像界面的槽
	void on_winAdjust(QPair<double, double> win);//需要更新直方图
	void on_autoWP();//需要更新直方图
	void on_curIndexChange(int index);//需要更新2个图像，

	//JOb界面的槽
	void on_jobDelectAll();
	void on_jobDelectItem();

	void on_updateCrossLines1(QPointF pos);
	void on_updateCrossLines2(QPointF pos);
	void on_copyImage();

signals:

private:
	void updateImages();
	void updateCrossLine1();
	void updateCrossLine2();
	
	//界面
	myTableView *jobTable;
	QTabWidget *paraWidget;//一个空的窗口,承接参数界面
	mykspaceview *rawDatagraphviewer;
	mykspaceview *fftDatagraphviewer;
	kspacepara *kspara;//参数控件
	myHistgram *histgram;//直方图

	//与界面有关，当前数据
	myJobItem curJobItem;
	ParaPro para;//FFT处理参数
	dataMode curData; //当前数据,不涉及原始数据，节约空间要清理掉
	ParaMRI curpara;//当前磁共振参数

	//局部变量，为了简便没有写成函数的局部变量
	AnnoItemGroup myAnnoTL;//当前标签
	AnnoItemGroup myAnnoBL;
	QPoint xyCrossPos1;//左边的十字线
	QPoint xyCrossPos2;// 左边的十字线

	DataImage curImage;
	DataImage curFFTimage;
	int curImageIndex=0;
	bool isFFTprocessed = false;
};
