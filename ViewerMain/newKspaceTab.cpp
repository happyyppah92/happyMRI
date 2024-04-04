#include "newKspaceTab.h"
#include "myFFTProcess.h"
#include <QDebug>

NewKspaceTab::NewKspaceTab(QWidget *parent)
	: QWidget(parent)
{
	Q_UNUSED(parent);
	//作业布局
	QVBoxLayout *jobLayout= new QVBoxLayout;
	jobTable = new myTableView;
	QHBoxLayout *btnLayout= new QHBoxLayout;
	QSpacerItem *spacer1;
	spacer1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
	jobLayout->addWidget(jobTable);
	jobLayout->addLayout(btnLayout);

	//参数布局
	paraWidget = new QTabWidget();
	QHBoxLayout *operatorLayout = new QHBoxLayout;
	histgram = new myHistgram;
	operatorLayout->addWidget(histgram);
	operatorLayout->addWidget(paraWidget);
	operatorLayout->setStretchFactor(histgram, 1);
	operatorLayout->setStretchFactor(paraWidget, 1);

	//图像显示，程序上面的2个界面
	QHBoxLayout *viewerLayout=new QHBoxLayout;
	rawDatagraphviewer = new mykspaceview;
	fftDatagraphviewer = new mykspaceview;
	viewerLayout->addWidget(rawDatagraphviewer);
	viewerLayout->addWidget(fftDatagraphviewer);
	viewerLayout->setStretchFactor(rawDatagraphviewer, 1);
	viewerLayout->setStretchFactor(fftDatagraphviewer, 1);
	viewerLayout->setSpacing(1);

	QVBoxLayout *mainLayout=new QVBoxLayout;
	mainLayout->addLayout(viewerLayout);
	mainLayout->addLayout(operatorLayout);
	mainLayout->setStretchFactor(viewerLayout, 7);
	mainLayout->setStretchFactor(operatorLayout, 3);

	QHBoxLayout *aa = new QHBoxLayout;
	aa->addLayout(mainLayout);
	aa->addLayout(jobLayout);
	aa->setStretchFactor(jobLayout, 2);
	aa->setStretchFactor(mainLayout, 8);
	setLayout(aa);
	rawDatagraphviewer->setFocus();

	kspara = new kspacepara(paraWidget);
	connect(jobTable, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(on_JobSelect(const QModelIndex &)));
	connect(kspara->ui.smoothchk, SIGNAL(stateChanged(int)), this, SLOT(on_smoothClick(int)));
	connect(kspara->ui.smoothtype, SIGNAL(currentIndexChanged(int)), this, SLOT(on_smoothClick(int)));
	connect(kspara->ui.smoothsize, SIGNAL(valueChanged(int)), this, SLOT(on_smoothClick(int)));

	connect(kspara->ui.windowchk, SIGNAL(stateChanged(int)), this, SLOT(on_windowClick(int)));
	connect(kspara->ui.windowtype, SIGNAL(currentIndexChanged(int)), this, SLOT(on_windowClick(int)));
	connect(kspara->ui.windowsize, SIGNAL(valueChanged(int)), this, SLOT(on_windowClick(int)));

	connect(kspara->ui.PFchk, SIGNAL(stateChanged(int)), this, SLOT(on_PFClick(int)));
	connect(kspara->ui.partialFouriertype, SIGNAL(currentIndexChanged(int)), this, SLOT(on_PFClick(int)));
	connect(kspara->ui.partialFouriePercent, SIGNAL(valueChanged(int)), this, SLOT(on_PFClick(int)));

	connect(kspara->ui.undersamplechk, SIGNAL(stateChanged(int)), this, SLOT(on_undersampleClick(int)));
	connect(kspara->ui.undersampletype, SIGNAL(currentIndexChanged(int)), this, SLOT(on_undersampleClick(int)));
	connect(kspara->ui.undersamplepercent, SIGNAL(valueChanged(int)), this, SLOT(on_undersampleClick(int)));

	connect(jobTable->delectAllItemAct, SIGNAL(triggered()), this, SLOT(on_jobDelectAll()));
	connect(jobTable->delectItemAct, SIGNAL(triggered()), this, SLOT(on_jobDelectItem()));

	connect(rawDatagraphviewer->mygv, SIGNAL(WinAdjust_signal(QPair<double, double>)), this, SLOT(on_winAdjust(QPair<double, double>)));
	connect(rawDatagraphviewer->mygv, SIGNAL(AutoWP_signal()), this, SLOT(on_autoWP()));

	connect(rawDatagraphviewer->mygv, SIGNAL(updatelines_signal(QPointF)), this, SLOT(on_updateCrossLines1(QPointF)));
	connect(fftDatagraphviewer->mygv, SIGNAL(updatelines_signal(QPointF)), this, SLOT(on_updateCrossLines2(QPointF)));

	connect(rawDatagraphviewer->mygv, SIGNAL(CopyImage_signal()), this, SLOT(on_copyImage()));//复制全部图像
	connect(fftDatagraphviewer->mygv, SIGNAL(CopyImage_signal()), this, SLOT(on_copyImage()));
}

NewKspaceTab::~NewKspaceTab()
{

}

void NewKspaceTab::on_jobDelectItem()
{
	//获取当前选中项
	QModelIndex curindex = jobTable->currentIndex();
	if (curindex.isValid()) {
		int row = curindex.row();//是从1开始的
		if (myJobList.size() > row) {
			myJobList.removeAt(row);
			jobTable->addNewJob(myJobList);
		}
		else
		{
		}
	}
}
void NewKspaceTab::on_jobDelectAll()
{
	myJobList.clear();
	jobTable->delectAll();
}


void NewKspaceTab::updateData()
{
	this->jobTable->addNewJob(myJobList);


}

void NewKspaceTab::on_JobSelect(const QModelIndex index)
{
	if (index.isValid()) {
		int row = index.row();//是从1开始的
		if (myJobList.size() > row) {
			curData = myJobList.at(row).curJobData;
			ParaMRI curparatemp;
			curparatemp = myJobList.at(row).curJobPara;
			curJobItem = myJobList.at(row);
			if (curJobItem.curJobResult.size() < curImageIndex+1)
			{
				on_curIndexChange(curJobItem.curJobResult.size()-1);
			}
			updateImages();
			rawDatagraphviewer->mygv->myfitinview();
			fftDatagraphviewer->mygv->myfitinview();
			xyCrossPos1.setX(curImage.width / 2);
			xyCrossPos1.setY(curImage.height / 2);
			xyCrossPos2.setX(curFFTimage.width / 2);
			xyCrossPos2.setY(curFFTimage.height / 2);
			histgram->setAutoY();
		}
		else 
		{
		}
	}
}

void NewKspaceTab::updateImages()
{
if (isFFTprocessed == false)
{
	if (curJobItem.curJobResult.size()>0)
	{
		isFFTprocessed = true;
		int imageWidth, imageHeight;
		imageWidth = curJobItem.curJobData.widthImage;
		imageHeight = curJobItem.curJobData.heightImage;

		//更新标签,
		myAnnoTL.itemsStr.clear();
		myAnnoTL.pos = QPointF(0, 0);

		myAnnoTL.itemsStr.append("Name:" + curJobItem.curJobData.fileName);//文件名
		myAnnoTL.itemsStr.append(curJobItem.getDateTime().toString("yyyy.MM.dd hh:mm:ss"));
		myAnnoTL.itemsStr.append(curJobItem.getJobSeqdescrip());
		myAnnoTL.itemsStr.append(QString::number(imageWidth) + " X "
			+ QString::number(imageHeight) + " X "
			+ curJobItem.getNumImage());
		myAnnoTL.itemsStr.append("TE:" + QString::number(curJobItem.curJobPara.TE) + " ms");
		myAnnoTL.itemsStr.append("TR:" + QString::number(curJobItem.curJobPara.TR) + " ms");
		myAnnoTL.itemsStr.append(SEQTYPE.at(curJobItem.curJobPara.Seqtype - 1));
		myAnnoTL.itemsStr.append("SCALE:1 X");

		myAnnoBL.pos = QPointF(0, -20);
		myAnnoBL.itemsStr.clear();
		myAnnoBL.itemsStr.append("Position:");//0
		myAnnoBL.itemsStr.append("Value:");//1

		QPair<int, int> sizeImage(imageWidth, imageHeight);
		myAnnoBL.itemsStr.append("WL: WW:");
		myAnnoBL.itemsStr.append("1/" + QString::number(curJobItem.curJobResult.size()));
		//FFT
		kspara->getProcessPara(para);
		DataImage image2FFT, imageAFFT, FFT;
		image2FFT.width = imageWidth;
		image2FFT.height = imageHeight;
		image2FFT.resultReal = curJobItem.curJobResult;
		myFFTProcess mfftprocess;

		mfftprocess.Cal2DFFTandProcess(image2FFT, para, imageAFFT, FFT);

		curImage = imageAFFT;
		curFFTimage = FFT;
		//第一个窗口
		rawDatagraphviewer->mygv->updateGraphyData(imageAFFT.resultReal, sizeImage);
		rawDatagraphviewer->mygv->updateAnnoData(myAnnoTL, cornerTypeEnum::TL);
		rawDatagraphviewer->mygv->updateAnnoData(myAnnoBL, cornerTypeEnum::BL);
		rawDatagraphviewer->mygv->myAutoWP();
		//rawDatagraphviewer->mygv->myfitinview();
		//第二个窗口
		myAnnoTL.itemsStr.clear();
		myAnnoTL.itemsStr.append("K-Space");
		myAnnoTL.itemsStr.append(QString::number(imageAFFT.width) + " X "
			+ QString::number(imageAFFT.height) + " X "
			+ curJobItem.getNumImage());

		fftDatagraphviewer->mygv->updateGraphyData(FFT.resultReal, QPair<int, int>(imageAFFT.width, imageAFFT.height));
		fftDatagraphviewer->mygv->updateAnnoData(myAnnoTL, cornerTypeEnum::TL);
		fftDatagraphviewer->mygv->updateAnnoData(myAnnoBL, cornerTypeEnum::BL);
		fftDatagraphviewer->mygv->myAutoWP();
		//fftDatagraphviewer->mygv->myfitinview();
		curImageIndex = 0;
		//计算曲线
		QVector<double >Result;
		Result = imageAFFT.resultReal.at(curImageIndex);;//输入当前图片
		rawDatagraphviewer->updataXYPlot(Result, imageWidth, imageHeight, imageWidth / 2, imageHeight / 2);

		Result = FFT.resultReal.at(curImageIndex);
		fftDatagraphviewer->updataXYPlot(Result, imageWidth, imageHeight, imageWidth / 2, imageHeight / 2);
		histgram->updateHistgarm(rawDatagraphviewer->mygv->myImage);

		image2FFT.resultReal.clear();
		imageAFFT.resultReal.clear();
		FFT.resultReal.clear();
		Result.clear();
	}
}
	isFFTprocessed = false;
}

void NewKspaceTab::on_smoothClick(int state)
{
	Q_UNUSED(state)
	updateImages();
}

void NewKspaceTab::on_windowClick(int state)
{
	Q_UNUSED(state)
	updateImages();
}

void NewKspaceTab::on_PFClick(int state)
{
	Q_UNUSED(state)
	updateImages();
}

void NewKspaceTab::on_undersampleClick(int state)
{
	Q_UNUSED(state)
	updateImages();
}


void NewKspaceTab::on_winAdjust(QPair<double, double> win)
{
	rawDatagraphviewer->mygv->setLinkWinAdjust(win);
	histgram->updateHistgarm(rawDatagraphviewer->mygv->myImage);
}

void NewKspaceTab::on_autoWP()
{
	rawDatagraphviewer->mygv->setLinkAutoWin();
	histgram->updateHistgarm(rawDatagraphviewer->mygv->myImage);
}

void NewKspaceTab::on_curIndexChange(int index)
{
	curImageIndex = index;
	rawDatagraphviewer->mygv->changeIndex(curImageIndex);//更新图像
	fftDatagraphviewer->mygv->changeIndex(curImageIndex);
	updateCrossLine1();
	updateCrossLine2();
	histgram->updateHistgarm(rawDatagraphviewer->mygv->myImage);
}

void NewKspaceTab::on_updateCrossLines1(QPointF pos)
{
	xyCrossPos1 = pos.toPoint();
	updateCrossLine1();
}
void NewKspaceTab::on_updateCrossLines2(QPointF pos)
{
	xyCrossPos2 = pos.toPoint();
	updateCrossLine2();
}


void NewKspaceTab::updateCrossLine1()
{
	QVector<double> Result = curImage.resultReal.at(curImageIndex);;//输入当前图片
	int imageWidth = curImage.width;
	int imageHeight = curImage.height;
	rawDatagraphviewer->updataXYPlot(Result, imageWidth, imageHeight, xyCrossPos1.x(), xyCrossPos1.y());
	Result.clear();
}
void NewKspaceTab::updateCrossLine2()
{
	QVector<double> Result = curFFTimage.resultReal.at(curImageIndex);;//输入当前图片
	int imageWidth = curImage.width;
	int imageHeight = curImage.height;
	fftDatagraphviewer->updataXYPlot(Result, imageWidth, imageHeight, xyCrossPos2.x(), xyCrossPos2.y());
	Result.clear();
}

void NewKspaceTab::on_copyImage()
{

	QClipboard* clip = QApplication::clipboard();
	QSize widsize1 = rawDatagraphviewer->size();
	QSize widsize2 = fftDatagraphviewer->size();
	QImage imageAll(widsize1.width() + widsize2.width(), widsize1.height(), QImage::Format_ARGB32);
	QPainter painter(&imageAll);
	rawDatagraphviewer->render(&painter, QPoint(0, 0), QRect(0, 0, widsize1.width(), widsize1.height()));   //关键函数
	fftDatagraphviewer->render(&painter, QPoint(widsize2.width(), 0), QRect(0, 0, widsize2.width(), widsize2.height()));   //关键函数
	clip->setImage(imageAll);
}

void NewKspaceTab::closePatient()
{
	curImage.resultReal.clear();
	curFFTimage.resultReal.clear();
	jobTable->closePatient();
	rawDatagraphviewer->mygv->clearGraphyView();
	fftDatagraphviewer->mygv->clearGraphyView();
}

