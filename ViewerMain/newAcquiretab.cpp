// 采集界面
#include "newAcquiretab.h"
#include "ui_mysequencediag.h"
#include "global.h"

#include <QtWidgets>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTransform>
#include <qpoint.h>

NewAquireTab::NewAquireTab(QWidget *parent)
{
	Q_UNUSED(parent);
	//按键布局
	sequenceBtn = new QPushButton("Sequence...");
	newBtn = new QPushButton("Scan");
	QHBoxLayout *btnLayout = new QHBoxLayout;
	QSpacerItem *spacer1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
	btnLayout->addWidget(newBtn);
	btnLayout->addWidget(sequenceBtn);
	btnLayout->addSpacerItem(spacer1);

	//作业布局
	QVBoxLayout *jobLayout = new QVBoxLayout;
	jobTable = new myTableView;
	jobLayout->addWidget(jobTable);
	jobLayout->addLayout(btnLayout);

	//参数布局
	paraWidget = new QTabWidget();
	manualgraph = new myGraphyView;
	manualgraph->setFocusPolicy(Qt::FocusPolicy::NoFocus);

	QHBoxLayout *operatorLayout = new QHBoxLayout;
	operatorLayout->addWidget(manualgraph);
	operatorLayout->addWidget(paraWidget);
	operatorLayout->addLayout(jobLayout);
	operatorLayout->setStretchFactor(jobLayout, 1);
	operatorLayout->setStretchFactor(paraWidget, 1);
	operatorLayout->setStretchFactor(manualgraph, 1);
	for (int i = 0; i < 3; ++i)
	{
		myGVwidget[i] = new QWidget;
		myGVlayout[i] = new QHBoxLayout(myGVwidget[i]);
		myGVlayout[i]->setMargin(0);
	}
	//图像显示，程序上面的3个界面
	QHBoxLayout *viewerLayout = new QHBoxLayout;
	graphviewer1 = new myGraphyView;
	graphviewer2 = new myGraphyView;
	graphviewer3 = new myGraphyView;
	QHBoxLayout temp1,temp2,temp3;
	myGVlayout[0]->addWidget(graphviewer1);
	myGVlayout[1]->addWidget(graphviewer2);
	myGVlayout[2]->addWidget(graphviewer3);
	viewerLayout->addWidget(myGVwidget[0]);
	viewerLayout->addWidget(myGVwidget[1]);
	viewerLayout->addWidget(myGVwidget[2]);
	viewerLayout->setStretchFactor(myGVwidget[0], 1);
	viewerLayout->setStretchFactor(myGVwidget[1], 1);
	viewerLayout->setStretchFactor(myGVwidget[2], 1);
	viewerLayout->setSpacing(1);

	// 全部
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(viewerLayout);
	mainLayout->addLayout(operatorLayout);
	mainLayout->setStretchFactor(viewerLayout, 5);
	mainLayout->setStretchFactor(operatorLayout, 3);
	setLayout(mainLayout);


	//布局设置完
	myAnnoTL.pos = QPointF(0, 0);
	myAnnoTL.itemsStr.append("Name:");
	myAnnoTL.itemsStr.append("TE:");
	myAnnoTL.itemsStr.append("TR:");
	myAnnoTL.itemsStr.append("SE:");
	myAnnoTL.itemsStr.append("SCALE:");
	myAnnoBL.pos = QPointF(0, 0);
	myAnnoBL.itemsStr.append("Position->()");
	myAnnoBL.itemsStr.append("Value->()");
	myAnnoBL.itemsStr.append("P->;W->");

	//变量初始化
	curFocus = 1;
	mysequence = new mySequenceView;
	myParaCtrl = new paraControl(paraWidget);//传入QTabWidget

	//界面初始化
	graphviewer1->setFocus();
	newBtn->setEnabled(false);

	//连接信号
	connect(newBtn, SIGNAL(clicked()), this, SLOT(on_startButton_clicked()));
	connect(sequenceBtn, SIGNAL(clicked()), this, SLOT(on_sequenceButton_clicked()));
	connect(graphviewer1, SIGNAL(setMyFocus_signal()), this, SLOT(on_view1getFocus()));
	connect(graphviewer2, SIGNAL(setMyFocus_signal()), this, SLOT(on_view2getFocus()));
	connect(graphviewer3, SIGNAL(setMyFocus_signal()), this, SLOT(on_view3getFocus()));
	connect(jobTable, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(on_JobSelect(const QModelIndex &)));
	connect(mysequence, SIGNAL(seqChange(seqTypeEnum)), this, SLOT(on_seqChange(seqTypeEnum)));
	connect(graphviewer1, SIGNAL(linkAllBroadcast_signal(bool,double,QPointF)), this, SLOT(on_linkAll(bool, double, QPointF)));
	connect(graphviewer2, SIGNAL(linkAllBroadcast_signal(bool, double, QPointF)), this, SLOT(on_linkAll(bool, double, QPointF)));
	connect(graphviewer3, SIGNAL(linkAllBroadcast_signal(bool, double, QPointF)), this, SLOT(on_linkAll(bool, double, QPointF)));

	connect(graphviewer1, SIGNAL(linkAllZoom_signal(double)), this, SLOT(on_linkAllzoom(double)));
	connect(graphviewer2, SIGNAL(linkAllZoom_signal(double)), this, SLOT(on_linkAllzoom(double)));
	connect(graphviewer3, SIGNAL(linkAllZoom_signal(double)), this, SLOT(on_linkAllzoom(double)));

	connect(graphviewer1, SIGNAL(linkAllValue_signal(QPoint)), this, SLOT(on_linkAllvalue(QPoint)));
	connect(graphviewer2, SIGNAL(linkAllValue_signal(QPoint)), this, SLOT(on_linkAllvalue(QPoint)));
	connect(graphviewer3, SIGNAL(linkAllValue_signal(QPoint)), this, SLOT(on_linkAllvalue(QPoint)));

	connect(graphviewer1, SIGNAL(linkALLMoveImage_signal(QPointF)), this, SLOT(on_linkAllMoveImage(QPointF)));
	connect(graphviewer2, SIGNAL(linkALLMoveImage_signal(QPointF)), this, SLOT(on_linkAllMoveImage(QPointF)));
	connect(graphviewer3, SIGNAL(linkALLMoveImage_signal(QPointF)), this, SLOT(on_linkAllMoveImage(QPointF)));

	connect(graphviewer1, SIGNAL(linkAllWinAdjust_signal(QPair<double, double>)), this, SLOT(on_linkAllwinAdjust(QPair<double, double>)));
	connect(graphviewer2, SIGNAL(linkAllWinAdjust_signal(QPair<double, double>)), this, SLOT(on_linkAllwinAdjust(QPair<double, double>)));
	connect(graphviewer3, SIGNAL(linkAllWinAdjust_signal(QPair<double, double>)), this, SLOT(on_linkAllwinAdjust(QPair<double, double>)));

	connect(graphviewer1, SIGNAL(linkAllAutoFitin_signal()), this, SLOT(on_linkAllfitin()));
	connect(graphviewer2, SIGNAL(linkAllAutoFitin_signal()), this, SLOT(on_linkAllfitin()));
	connect(graphviewer3, SIGNAL(linkAllAutoFitin_signal()), this, SLOT(on_linkAllfitin()));

	connect(graphviewer1, SIGNAL(linkAllAutoWP_signal()), this, SLOT(on_linkAllautoWP()));
	connect(graphviewer2, SIGNAL(linkAllAutoWP_signal()), this, SLOT(on_linkAllautoWP()));
	connect(graphviewer3, SIGNAL(linkAllAutoWP_signal()), this, SLOT(on_linkAllautoWP()));

	connect(graphviewer1, SIGNAL(linkAllCopyImage_signal()), this, SLOT(on_linkAllcopyImage()));
	connect(graphviewer2, SIGNAL(linkAllCopyImage_signal()), this, SLOT(on_linkAllcopyImage()));
	connect(graphviewer3, SIGNAL(linkAllCopyImage_signal()), this, SLOT(on_linkAllcopyImage()));

	connect(graphviewer1, SIGNAL(linkAllChangeIndex(int)), this, SLOT(on_linkAllIndexChanged(int)));
	connect(graphviewer2, SIGNAL(linkAllChangeIndex(int)), this, SLOT(on_linkAllIndexChanged(int)));
	connect(graphviewer3, SIGNAL(linkAllChangeIndex(int)), this, SLOT(on_linkAllIndexChanged(int)));

	connect(graphviewer1, SIGNAL(CopyImage_signal()), this, SLOT(on_copyImage1()));//复制全部图像
	connect(graphviewer2, SIGNAL(CopyImage_signal()), this, SLOT(on_copyImage2()));
	connect(graphviewer3, SIGNAL(CopyImage_signal()), this, SLOT(on_copyImage3()));

	connect(jobTable->delectAllItemAct, SIGNAL(triggered()), this, SLOT(on_jobDelectAll()));
	connect(jobTable->delectItemAct, SIGNAL(triggered()), this, SLOT(on_jobDelectItem()));
}

void NewAquireTab::updateParaFrom(seqTypeEnum type)
{
	QString curPath = QCoreApplication::applicationDirPath();
	QString strPath;
	QString manualPath;
	if (type == seqTypeEnum::SE) {
		strPath = curPath + "/Sequence/se.xml";
		manualPath = curPath + "/Sequence/SE.PNG";
	}
	else if (type == seqTypeEnum::IR) {
		strPath = curPath + "/Sequence/ir.xml";
		manualPath = curPath + "/Sequence/IR.PNG";
	}

	paraWidget->clear();//主界面上的参数界面
	myParaCtrl->CreateUIFromXml(strPath);

	QPixmap manuImage(manualPath);
	manualgraph->updateMyImage(manuImage);
	QGraphicsScene myGraphyScene;
	myGraphyScene.addPixmap(manuImage);
	myGraphyScene.setSceneRect(manuImage.rect());
	manualgraph->fitInView(myGraphyScene.sceneRect(), Qt::IgnoreAspectRatio);
}

int NewAquireTab::updateData(dataMode current)
{
	//打开数据后 1.1将数据拷贝
	curData = current.copyValue();
	//在job窗口显示,//数据为Map数据
	if (current.dataType == dataTypeEnum::MAP2D || current.dataType == dataTypeEnum::MAP3D) {
		//加入Job1
		myJobItem tempjob;
		curpara.Seqtype = seqTypeEnum::PDValue;
		tempjob.newJob("", QDateTime::currentDateTime(), curData, curpara);
		tempjob.setJobSeqdescrip("PD value");
		tempjob.setJobHead("");
		tempjob.curJobResult = curData.PDmap;//将PD data添加到job      
		myJobList.append(tempjob); //数据放在 JobList中  
		jobTable->addNewJob(myJobList); //将JobList更新到 Jobtable中
		curFocus = 1;
		showGraphviewwerByJob(tempjob);//显示

		//加入Job2
		curpara.Seqtype = seqTypeEnum::T1Value;
		tempjob.newJob("", QDateTime::currentDateTime(), curData, curpara);
		tempjob.setJobSeqdescrip("T1 value");
		tempjob.setJobHead("");
		tempjob.curJobResult = curData.T1map;//将PD data添加到job									 
		myJobList.append(tempjob);//数据放在 JobList中
		jobTable->addNewJob(myJobList);//将JobList更新到 Jobtable中
		curFocus = 2;
		showGraphviewwerByJob(tempjob);//显示

		//加入Job3
		curpara.Seqtype = seqTypeEnum::T2Value;
		tempjob.newJob("", QDateTime::currentDateTime(), curData, curpara);
		tempjob.setJobSeqdescrip("T2 value");
		tempjob.setJobHead("");
		tempjob.curJobResult = curData.T2map;//将PD data添加到job											
		myJobList.append(tempjob); //数据放在 JobList中
		jobTable->addNewJob(myJobList);		//将JobList更新到 Jobtable中
		curFocus = 3;
		showGraphviewwerByJob(tempjob);//显示

		curpara.Seqtype = seqTypeEnum::SE;
		updateParaFrom(curpara.Seqtype);//根据当前
		emit updateStatus("New Job Add", msgClassEnum::GREEN);

		tempjob.curJobData.clear();
		tempjob.curJobResult.clear();
	}
	else if (current.dataType == dataTypeEnum::Image2D) {

		//加入Job1
		myJobItem tempjob;
		tempjob.newJob("Image: 2D", QDateTime::currentDateTime(), curData, curpara);
		tempjob.curJobResult = curData.PDmap;
		//数据放在 JobList中
		myJobList.append(tempjob);
		//将JobList更新到 Jobtable中
		jobTable->addNewJob(myJobList);
		showGraphviewwerByJob(tempjob);
		emit updateStatus("New Job Add", msgClassEnum::GREEN);
		tempjob.curJobData.clear();
		tempjob.curJobResult.clear();
	}
	else if (current.dataType == dataTypeEnum::Image3D) {

	}
	else if (current.dataType == dataTypeEnum::ImageTime) {//只放在PDmap中,时间序列
		//加入Job1
		myJobItem tempjob;
		qDebug() << "current.PDmap" << current.PDmap.size();
		tempjob.newJob("Image: Time series", QDateTime::currentDateTime(), curData, curpara);
		tempjob.curJobResult = curData.PDmap;
		//数据放在 JobList中
		myJobList.append(tempjob);
		//将JobList更新到 Jobtable中
		jobTable->addNewJob(myJobList);

		showGraphviewwerByJob(tempjob);
		emit updateStatus("New Job Add", msgClassEnum::GREEN);
		tempjob.curJobData.clear();
		tempjob.curJobResult.clear();
	}

	curpara.Seqtype = seqTypeEnum::SE;//默认是SE
	return proStatuEnum::OK;
}

void NewAquireTab::on_startButton_clicked()
{
	//默认的参数
	curpara.TE = 10;//单位 ms
	curpara.TR = 100;
	QString sqename;
	QString curdescription = "";

	//加入Job
	myJobItem tempjob;
	//读参数
	if (curpara.Seqtype == seqTypeEnum::SE)
	{
		myParaCtrl->GetSEPara(curpara);
		tempjob.setJobHead(SEQTYPE.at(curpara.Seqtype - 1));
	}
	else if (curpara.Seqtype == seqTypeEnum::IR)
	{
		myParaCtrl->GetIRPara(curpara);
		tempjob.setJobHead(SEQTYPE.at(curpara.Seqtype - 1));
	}

	//计算回波信号
	QList<QVector<double>> SEVector;
	CalEchoSignal(curData, curpara, SEVector);//3D

	if (curpara.Seqtype == seqTypeEnum::SE) { sqename = "SE"; }
	if (curpara.Seqtype == seqTypeEnum::IR) { sqename = "IR"; }

	curdescription = curpara.description;
	tempjob.newJob(sqename, QDateTime::currentDateTime(), curData, curpara);//jobclass会自动写
	if (curpara.Seqtype == seqTypeEnum::SE)
	{
		if (curdescription.size() == 0) {//
			tempjob.setJobSeqdescrip(sqename + "->TE:" + QString::number(curpara.TE) + "ms,TR:" + QString::number(curpara.TR) + "ms");
		}
		else {
			curdescription.replace("#TE", QString::number(curpara.TE));
			curdescription.replace("#TR", QString::number(curpara.TR));
			tempjob.setJobSeqdescrip(curdescription);
		}
	}
	else if (curpara.Seqtype == seqTypeEnum::IR)
	{
		if (curdescription.size() == 0)
		{//
			tempjob.setJobSeqdescrip(sqename + "->TE:" +
				QString::number(curpara.TE) + "ms,TR:" +
				QString::number(curpara.TR) + "ms,TI:" +
				QString::number(curpara.TI));
		}
		else
		{
			curdescription.replace("#TE", QString::number(curpara.TE), Qt::CaseInsensitive);
			curdescription.replace("#TR", QString::number(curpara.TR), Qt::CaseInsensitive);
			curdescription.replace("#TI", QString::number(curpara.TI), Qt::CaseInsensitive);
			tempjob.setJobSeqdescrip(curdescription);
		}
	}
	tempjob.curJobPara = curpara;
	tempjob.curJobData = curData;
	tempjob.curJobResult = SEVector;
	myJobList.append(tempjob);
	jobTable->addNewJob(myJobList);

	showGraphviewwerByJob(tempjob);

	tempjob.curJobData.clear();
	tempjob.curJobResult.clear();
}

void NewAquireTab::on_view1getFocus()
{
	curFocus = 1;
}
void NewAquireTab::on_view2getFocus()
{
	curFocus = 2;
}
void NewAquireTab::on_view3getFocus()
{
	curFocus = 3;
}

void NewAquireTab::on_JobSelect(const QModelIndex index)
{
	if (index.isValid()) {
		int row = index.row();//是从1开始的
		//row是从0开始的
		if (myJobList.size() > row) {
			curData = myJobList.at(row).curJobData;
			ParaMRI curparatemp;
			curparatemp = myJobList.at(row).curJobPara;
			if (curparatemp.Seqtype != seqTypeEnum::PDValue &&
				curparatemp.Seqtype != seqTypeEnum::T1Value &&
				curparatemp.Seqtype != seqTypeEnum::T2Value)
			{
				curpara = curparatemp;
				updateParaFrom(curpara.Seqtype);
				myParaCtrl->ApplyParaValue(curpara);
			}
			else
			{
				//不变
				//curpara=默认参数；
			}
			//更新显示窗口
			showGraphviewwerByJob(myJobList.at(row));
		}
		else {
		}
	}
}

void NewAquireTab::showGraphviewwerByJob(myJobItem job)
{
	//初步转化成图像,获取位置,只是为了获取图像的大小
	//更新标签,
	myAnnoTL.itemsStr.clear();
	myAnnoTL.pos = QPointF(0, 0);

	myAnnoTL.itemsStr.append("Name:" + job.curJobData.fileName);//文件名
	myAnnoTL.itemsStr.append(job.getDateTime().toString("yyyy.MM.dd hh:mm:ss"));
	myAnnoTL.itemsStr.append(job.getJobSeqdescrip());
	myAnnoTL.itemsStr.append(QString::number(job.curJobData.widthImage) + " X "
		+ QString::number(job.curJobData.heightImage) + " X "
		+ job.getNumImage());
	myAnnoTL.itemsStr.append("TE:" + QString::number(job.curJobPara.TE) + " ms");
	myAnnoTL.itemsStr.append("TR:" + QString::number(job.curJobPara.TR) + " ms");
	myAnnoTL.itemsStr.append(SEQTYPE.at(job.curJobPara.Seqtype - 1));
	myAnnoTL.itemsStr.append("SCALE:1 X");

	myAnnoBL.pos = QPointF(0, -20);
	myAnnoBL.itemsStr.clear();
	myAnnoBL.itemsStr.append("Position:");//0
	myAnnoBL.itemsStr.append("Value:");//1

	//判断哪个窗口是 激活的，自动归一化
	QPair<int, int> sizeImage(job.curJobData.widthImage, job.curJobData.heightImage);
	myAnnoBL.itemsStr.append("WL: WW:");
	myAnnoBL.itemsStr.append("1/" + QString::number(job.curJobResult.size()));
	switch (curFocus) {
	case 3:
		graphviewer3->updateGraphyData(job.curJobResult, sizeImage);
		graphviewer3->updateAnnoData(myAnnoTL, cornerTypeEnum::TL);
		graphviewer3->updateAnnoData(myAnnoBL, cornerTypeEnum::BL);
		graphviewer3->myAutoWP();
		graphviewer3->myfitinview();
		break;
	case 2:
		graphviewer2->updateGraphyData(job.curJobResult, sizeImage);
		graphviewer2->updateAnnoData(myAnnoTL, cornerTypeEnum::TL);
		graphviewer2->updateAnnoData(myAnnoBL, cornerTypeEnum::BL);
		graphviewer2->myAutoWP();
		graphviewer2->myfitinview();
		break;
	default:
		graphviewer1->updateGraphyData(job.curJobResult, sizeImage);
		graphviewer1->updateAnnoData(myAnnoTL, cornerTypeEnum::TL);
		graphviewer1->updateAnnoData(myAnnoBL, cornerTypeEnum::BL);
		graphviewer1->myAutoWP();
		graphviewer1->myfitinview();
		break;
	}
}

void NewAquireTab::on_sequenceButton_clicked()
{
	mysequence->show();
	emit updateStatus("Selected a sequence", msgClassEnum::NORMAL);
}

void NewAquireTab::on_seqChange(seqTypeEnum seq)
{
	curpara.Seqtype = seq;
	if (curpara.Seqtype == seqTypeEnum::SE) {
		emit updateStatus("SE Selected", msgClassEnum::GREEN);
	}
	else if (curpara.Seqtype == seqTypeEnum::IR) {
		emit updateStatus("IR Selected", msgClassEnum::GREEN);
	}

	updateParaFrom(seq);
}

void NewAquireTab::closepatient()
{
	graphviewer1->clearGraphyView();
	graphviewer2->clearGraphyView();
	graphviewer3->clearGraphyView();
	manualgraph->clearGraphyView();
	jobTable->closePatient();

	newBtn->setEnabled(false);
	myJobList.clear();
	curData.clear();
	myAnnoTL.itemsStr.clear();
	myAnnoBL.itemsStr.clear();
}

void NewAquireTab::on_linkAll(bool stu,double scale,QPointF barPos)
{
	graphviewer1->setLinkstyle(stu);
	graphviewer2->setLinkstyle(stu);
	graphviewer3->setLinkstyle(stu);
	isLinkallMode = stu;
	if (stu == true)
	{
		//第一遍设置成一样的放大倍数，这样移动才是一样的
		graphviewer1->setZoom(scale);
		graphviewer2->setZoom(scale);
		graphviewer3->setZoom(scale);

		graphviewer1->horizontalScrollBar()->setValue(barPos.x());
		graphviewer2->horizontalScrollBar()->setValue(barPos.x());
		graphviewer3->horizontalScrollBar()->setValue(barPos.x());

		graphviewer1->verticalScrollBar()->setValue(barPos.y());
		graphviewer2->verticalScrollBar()->setValue(barPos.y());
		graphviewer3->verticalScrollBar()->setValue(barPos.y());

		//第二遍用于更新标签
		graphviewer1->setZoom(scale);
		graphviewer2->setZoom(scale);
		graphviewer3->setZoom(scale);
	}
}

void NewAquireTab::on_linkAllzoom(double scale)
{
	graphviewer1->setZoom(scale);
	graphviewer2->setZoom(scale);
	graphviewer3->setZoom(scale);
}

void NewAquireTab::on_linkAllvalue(QPoint pos)
{
	graphviewer1->setLinkValue(pos);
	graphviewer2->setLinkValue(pos);
	graphviewer3->setLinkValue(pos);
}

void NewAquireTab::on_linkAllwinAdjust(QPair<double, double> win) {
	graphviewer1->setLinkWinAdjust(win);
	graphviewer2->setLinkWinAdjust(win);
	graphviewer3->setLinkWinAdjust(win);
}

void NewAquireTab::on_linkAllfitin()
{
	graphviewer1->setLinkFitin();
	graphviewer2->setLinkFitin();
	graphviewer3->setLinkFitin();
}

void NewAquireTab::on_linkAllautoWP()
{
	graphviewer1->setLinkAutoWin();
	graphviewer2->setLinkAutoWin();
	graphviewer3->setLinkAutoWin();
}

void NewAquireTab::on_linkAllIndexChanged(int index)
{
	graphviewer1->changeIndex(index);
	graphviewer2->changeIndex(index);
	graphviewer3->changeIndex(index);
}

void NewAquireTab::on_linkAllMoveImage(QPointF pos)
{
	graphviewer1->setLinkMoveImage(pos);
	graphviewer2->setLinkMoveImage(pos);
	graphviewer3->setLinkMoveImage(pos);
}
void NewAquireTab::on_linkAllcopyImage()
{
	QClipboard *clip = QApplication::clipboard();
	QSize widsize = myGVwidget[0]->size();
	QImage imageAll(widsize.width() * 3, widsize.height(), QImage::Format_ARGB32);
	QPainter painter(&imageAll);
	myGVwidget[0]->render(&painter, QPoint(0, 0), QRect(0, 0, widsize.width(), widsize.height()));
	myGVwidget[1]->render(&painter, QPoint(widsize.width(), 0), QRect(0, 0, widsize.width(), widsize.height()));
	myGVwidget[2]->render(&painter, QPoint(widsize.width()*2, 0), QRect(0, 0, widsize.width(), widsize.height()));
	clip->setImage(imageAll);
}
void NewAquireTab::on_copyImage1()
{
	QClipboard* clip = QApplication::clipboard();
	QSize widsize = myGVwidget[0]->size();
	QImage imageAll(widsize.width(), widsize.height(), QImage::Format_ARGB32);
	QPainter painter(&imageAll);
	myGVwidget[0]->render(&painter, QPoint(0, 0), QRect(0, 0, widsize.width(), widsize.height()));
	clip->setImage(imageAll);
}

void NewAquireTab::on_copyImage2()
{
	QClipboard* clip = QApplication::clipboard();
	QSize widsize = myGVwidget[1]->size();
	QImage imageAll(widsize.width(), widsize.height(), QImage::Format_ARGB32);
	QPainter painter(&imageAll);
	myGVwidget[1]->render(&painter, QPoint(0, 0), QRect(0, 0, widsize.width(), widsize.height()));
	clip->setImage(imageAll);
}

void NewAquireTab::on_copyImage3()
{
	QClipboard* clip = QApplication::clipboard();
	QSize widsize = myGVwidget[2]->size();
	QImage imageAll(widsize.width(), widsize.height(), QImage::Format_ARGB32);
	QPainter painter(&imageAll);
	myGVwidget[2]->render(&painter, QPoint(0, 0), QRect(0, 0, widsize.width(), widsize.height()));
	clip->setImage(imageAll);
}

void NewAquireTab::on_jobDelectAll()
{
	myJobList.clear();
	jobTable->delectAll();
}

void NewAquireTab::on_jobDelectItem()
{
	//获取当前选中项
	QModelIndex curindex = jobTable->currentIndex();
	if (curindex.isValid())
	{
		int row = curindex.row();//是从1开始的
		//row是从0开始的
		if (myJobList.size() > row) 
		{
			myJobItem temp;
			temp=myJobList.takeAt(row);//移除后要手动销毁
			jobTable->addNewJob(myJobList);
		}
		else 
		{
		}
	}
}
