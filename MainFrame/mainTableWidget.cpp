// 主界面,菜单下面的那部分
#include "MainTableWidget.h"
#include "global.h"
#include <QtWidgets>
#include <QFont>


MainTableWidget::MainTableWidget(QWidget *parent)
    : QTabWidget(parent)   
{
    setupTabs();
	//建立数据传输通道
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}


void MainTableWidget::setupTabs()
{
	AcquireTab = new NewAquireTab(this);
	addTab(AcquireTab, "Imaging");//index=0
	PhysicsTab = new NewPhysicsTab(this);
	addTab(PhysicsTab, "Physics");//index=1
	KSpaceTab = new NewKspaceTab(this);
	addTab(KSpaceTab, "K-Space");//index=2
    ViewrTab = new newViewertab(this);
	addTab(ViewrTab, "Viewer");//index=3

    this->tabBar()->setProperty("class","1");//设置为1
    this->setTabPosition(QTabWidget::East);
    this->setTabShape(QTabWidget::Triangular);
/*	AcquireTab->layout()->setMargin(0);
	PhysicsTab->layout()->setMargin(0);
	KSpaceTab->layout()->setMargin(0);
	ViewrTab->layout()->setMargin(0)*/;

}

void MainTableWidget::tabChanged(int index)
{
	switch (index)
	{
	case 0: emit setupMainWindows(TabnameEnum::ACQ); break;
	case 1: emit setupMainWindows(TabnameEnum::PHY); break;
	case 2: emit setupMainWindows(TabnameEnum::KSPACE); break;
	case 3: emit setupMainWindows(TabnameEnum::VIEWER); break;
	default:emit setupMainWindows(TabnameEnum::ACQ); break;
	}

	//传输数据
	if (1==index)
	{
		tranDataFromAcq2physic();
	}
	if (2==index)
	{
		tranDataFromAcq2kspace();
	}
}

//将数据传递到 kspace
void MainTableWidget::tranDataFromAcq2kspace()
{
	//只传递结果数据
	int acqjobsize = AcquireTab->myJobList.size();
	KSpaceTab->myJobList.clear();
	for (int ijob = 0; ijob < acqjobsize; ++ijob)
	{
		myJobItem temp;
		temp = AcquireTab->myJobList.at(ijob);
		temp.curJobData.PDmap.clear();//注意这里的数据不传
		temp.curJobData.T1map.clear();
		temp.curJobData.T2map.clear();
		temp.curJobData.T2Startmap.clear();
		//只传递序列结果
		if (temp.curJobPara.Seqtype == seqTypeEnum::SE)
		{
			KSpaceTab->myJobList.append(temp);
		}
		else if (temp.curJobPara.Seqtype == seqTypeEnum::IR)
		{
			KSpaceTab->myJobList.append(temp);
		}
		temp.curJobResult.clear();
	}
	KSpaceTab->updateData();
}

void MainTableWidget::tranDataFromAcq2physic()
{
	//只传递原始数据
	if (AcquireTab->myJobList.size() > 0)
	{
		dataMode currentData = AcquireTab->myJobList.at(0).curJobData;
		PhysicsTab->updateData(currentData);
	}

}

