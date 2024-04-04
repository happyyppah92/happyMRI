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

	//��������Ĳ�
	void on_smoothClick(int);
	void on_windowClick(int);
	void on_PFClick(int);
	void on_undersampleClick(int);

	//ͼ�����Ĳ�
	void on_winAdjust(QPair<double, double> win);//��Ҫ����ֱ��ͼ
	void on_autoWP();//��Ҫ����ֱ��ͼ
	void on_curIndexChange(int index);//��Ҫ����2��ͼ��

	//JOb����Ĳ�
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
	
	//����
	myTableView *jobTable;
	QTabWidget *paraWidget;//һ���յĴ���,�нӲ�������
	mykspaceview *rawDatagraphviewer;
	mykspaceview *fftDatagraphviewer;
	kspacepara *kspara;//�����ؼ�
	myHistgram *histgram;//ֱ��ͼ

	//������йأ���ǰ����
	myJobItem curJobItem;
	ParaPro para;//FFT�������
	dataMode curData; //��ǰ����,���漰ԭʼ���ݣ���Լ�ռ�Ҫ�����
	ParaMRI curpara;//��ǰ�Ź������

	//�ֲ�������Ϊ�˼��û��д�ɺ����ľֲ�����
	AnnoItemGroup myAnnoTL;//��ǰ��ǩ
	AnnoItemGroup myAnnoBL;
	QPoint xyCrossPos1;//��ߵ�ʮ����
	QPoint xyCrossPos2;// ��ߵ�ʮ����

	DataImage curImage;
	DataImage curFFTimage;
	int curImageIndex=0;
	bool isFFTprocessed = false;
};
