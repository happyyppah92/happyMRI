#ifndef NewAquireTab_H
#define NewAquireTab_H

#include "dataMode.h"
#include "mytableview.h"
#include "myGraphyView.h"
#include "paraControl.h"
#include "mySequenceView.h"
#include "myProcess.h"
#include "global.h"

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QTableView>
#include <QTabWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

class NewAquireTab : public QWidget
{
    Q_OBJECT

public:
    NewAquireTab(QWidget *parent = 0);
	int updateData(dataMode data);//将值传过来即可
	void updateParaFrom(seqTypeEnum type);
    void showGraphviewwerByJob(myJobItem job);//用于显示，不做更改，传递值
    void closepatient();
    QList<myJobItem > myJobList;
    QPushButton *sequenceBtn;
    QPushButton *newBtn;
    bool isLinkallMode=0;

public slots:
	 //LinkAll下槽，用于接收下级的消息
     void on_linkAll(bool stu, double scale, QPointF barPos);
     void on_linkAllzoom(double scale);
     void on_linkAllvalue(QPoint pos);
     void on_linkAllwinAdjust(QPair<double, double> win);
     void on_linkAllfitin();
     void on_linkAllautoWP();
     void on_linkAllcopyImage();
	 void on_linkAllIndexChanged(int index);
	 void on_linkAllMoveImage(QPointF pos);
	 void on_copyImage1();
	 void on_copyImage2();
	 void on_copyImage3();

private slots:
	 //按键
	 void on_startButton_clicked();
	 void on_sequenceButton_clicked();
	 void on_seqChange(seqTypeEnum seq);
	 //窗口
	 void on_view1getFocus();
	 void on_view2getFocus();
	 void on_view3getFocus();

	 //Job
	 void on_JobSelect(const QModelIndex index);
	 void on_jobDelectAll();
	 void on_jobDelectItem();

signals:
     void updateStatus(QString msg,msgClassEnum msgclass);//发射状态

private:
	paraControl *myParaCtrl;//
	//界面
    myTableView *jobTable;
    QTabWidget *paraWidget;//一个空的窗口
	//4个图像显示区域
    myGraphyView *manualgraph;
    myGraphyView *graphviewer1;
	QWidget* myGVwidget[3] = {};
	QHBoxLayout* myGVlayout[3] = {};
    myGraphyView *graphviewer2;
    myGraphyView *graphviewer3;

    //参数界面
    mySequenceView *mysequence;
	//与界面有关，当前数据
    dataMode curData; //当前数据
	ParaMRI curpara;//当前磁共振参数

	//全局参数
	unsigned int curFocus=0;//当前视图
	//局部变量，为了简便没有写成函数的局部变量
	AnnoItemGroup myAnnoTL;//当前标签
	AnnoItemGroup myAnnoBL;
};

#endif
