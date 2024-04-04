#include "newPhysicsTab.h"
#include "global.h"
#include "myProcess.h"
#include "mySECurve.h"
#include <QPair>

NewPhysicsTab::NewPhysicsTab(QWidget *parent)
	: QWidget(parent)
{
	Q_UNUSED(parent)

	//roi布局
	QVBoxLayout *ROILayout = new QVBoxLayout;
	roiTable = new myRoitableview;
	QHBoxLayout *btnLayout = new QHBoxLayout;
	addRoiBtn = new QPushButton("Add ROI");
	QSpacerItem *spacer1;
	spacer1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
	btnLayout->addWidget(addRoiBtn);
	btnLayout->addSpacerItem(spacer1);
	ROILayout->addWidget(roiTable);
	ROILayout->addLayout(btnLayout);

	//操作布局
	QHBoxLayout *curveLayout = new QHBoxLayout;
	SECurve = new mySECurve;
	phyPara = new physicpara;
	curveLayout->addWidget(SECurve);
	curveLayout->addWidget(phyPara);
	curveLayout->setStretchFactor(SECurve, 2);
	curveLayout->setStretchFactor(phyPara, 1);

	TEvalue = 20;
	TRvalue = 1000;
	phyPara->ui.TESlider->setValue(TEvalue);
	phyPara->ui.TEvalueLabel->setText(QString::number(TEvalue) + "ms");
	phyPara->ui.TESlider->setRange(0, TEmax);
	phyPara->ui.TRSlider->setValue(TRvalue);
	phyPara->ui.TRvalueLabel->setText(QString::number(TRvalue) + "ms");
	phyPara->ui.TRSlider->setRange(0, TRmax);
	phyPara->ui.autoWPcheckBox->setTristate(false);
	phyPara->ui.autoWPcheckBox->setChecked(false);
	isGetWPWin = true;


	//图像显示，程序上面的2个界面
	QHBoxLayout *viewerLayout = new QHBoxLayout;
	rawDataView = new mykspaceview;
	MRIdataView = new mykspaceview;
	viewerLayout->addWidget(rawDataView);
	viewerLayout->addWidget(MRIdataView);
	viewerLayout->setStretchFactor(rawDataView, 1);
	viewerLayout->setStretchFactor(MRIdataView, 1);
	viewerLayout->setSpacing(1);

	//mainView
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(viewerLayout);
	mainLayout->addLayout(curveLayout);
	mainLayout->setStretchFactor(viewerLayout, 7);
	mainLayout->setStretchFactor(curveLayout, 3);

	QHBoxLayout *aa = new QHBoxLayout;
	aa->addLayout(mainLayout);
	aa->addLayout(ROILayout);
	aa->setStretchFactor(ROILayout, 2);
	aa->setStretchFactor(mainLayout, 8);
	setLayout(aa);

	rawDataView->setFocus();

	connect(addRoiBtn, SIGNAL(clicked()), this, SLOT(on_addROIBtnClicked()));

	connect(rawDataView->mygv, SIGNAL(updatelines_signal(QPointF)), this, SLOT(on_updateCrossLines(QPointF)));
	connect(MRIdataView->mygv, SIGNAL(updatelines_signal(QPointF)), this, SLOT(on_updateCrossLines(QPointF)));
	connect(rawDataView->mygv, SIGNAL(roiSelect_signal(QPointF)), this, SLOT(on_roiSelect(QPointF)));
	connect(MRIdataView->mygv, SIGNAL(roiSelect_signal(QPointF)), this, SLOT(on_roiSelect(QPointF)));

	connect(rawDataView->mygv, SIGNAL(CopyImage_signal()), this, SLOT(on_copyImage()));
	connect(MRIdataView->mygv, SIGNAL(CopyImage_signal()), this, SLOT(on_copyImage()));

	connect(phyPara->ui.TESlider, SIGNAL(valueChanged(int)), this, SLOT(on_TESliderChanged(int)));
	connect(phyPara->ui.TRSlider, SIGNAL(valueChanged(int)), this, SLOT(on_TRSliderChanged(int)));
	connect(phyPara->ui.autoWPcheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_autoWPcheckStateChanged(int)));
	connect(roiTable->delectAllItemAct, SIGNAL(triggered()), this, SLOT(on_jobDelectAll()));
	connect(roiTable->delectItemAct, SIGNAL(triggered()), this, SLOT(on_jobDelectItem()));
	connect(SECurve->curvePlot, SIGNAL(legendDoubleClick(QCPLegend*, QCPAbstractLegendItem*, QMouseEvent*)), this, SLOT(on_legendDoubleClick(QCPLegend*, QCPAbstractLegendItem*)));
}

int NewPhysicsTab::updateData(dataMode current)
{
	if (current.dataType == dataTypeEnum::MAP2D)
	{
		////打开数据后 1.1将数据拷贝
		currawImage.resultReal.clear();
		currawImage.width = 0;
		currawImage.height = 0;
		curData = current.copyValue();
		if (current.dataType == dataTypeEnum::MAP2D)
		{
			emit updateStatus("New data Add", msgClassEnum::GREEN);
		}
		if (curData.PDmap.size() > 0)
		{
			currawImage.resultReal.append(curData.PDmap.at(0));
			currawImage.resultReal.append(curData.T1map.at(0));
			currawImage.resultReal.append(curData.T2map.at(0));
		}
		currawImage.width = curData.widthImage;
		currawImage.height = curData.heightImage;

		updateImages();
		rawDataView->mygv->myfitinview();
		updateMRIdataView();
		MRIdataView->mygv->myfitinview();
		return 1;
	}
	else
	{
		return 0;
	}
}

void NewPhysicsTab::updateImages()
{
	int imageWidth, imageHeight;
	imageWidth = currawImage.width;
	imageHeight = currawImage.height;

	QPair<int, int> sizeImage(imageWidth, imageHeight);
	//第一个窗口
	rawDataView->mygv->updateGraphyData(currawImage.resultReal, sizeImage);
	//更新标签,
	myAnnoTL.itemsStr.clear();
	myAnnoTL.pos = QPointF(0, 0);
	myAnnoTL.itemsStr.append("Name:" + curData.fileName);//文件名
	myAnnoTL.itemsStr.append("PD,T1 and T2 value");
	myAnnoTL.itemsStr.append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
	myAnnoTL.itemsStr.append(QString::number(curData.widthImage) + " X "
		+ QString::number(curData.heightImage));
	myAnnoTL.itemsStr.append("TE:");
	myAnnoTL.itemsStr.append("TR:");
	myAnnoTL.itemsStr.append("Raw data");
	myAnnoTL.itemsStr.append("SCALE:1 X");

	myAnnoBL.pos = QPointF(0, -20);
	myAnnoBL.itemsStr.clear();
	myAnnoBL.itemsStr.append("Position:");//0
	myAnnoBL.itemsStr.append("Value:");//1
	myAnnoBL.itemsStr.append("WL: WW:");
	myAnnoBL.itemsStr.append("");
	rawDataView->mygv->updateAnnoData(myAnnoTL, cornerTypeEnum::TL);
	rawDataView->mygv->updateAnnoData(myAnnoBL, cornerTypeEnum::BL);


	rawDataView->mygv->myAutoWP();

	xyCrossPos1.setX(currawImage.width / 2);
	xyCrossPos1.setY(currawImage.height / 2);

	QVector<double > Result = currawImage.resultReal.at(curImageIndex);;//输入当前图片
	rawDataView->updataXYPlot(Result, imageWidth, imageHeight, imageWidth / 2, imageHeight / 2);
}

void NewPhysicsTab::on_updateCrossLines(QPointF pos)
{
	xyCrossPos1 = pos.toPoint();
	updateCrossLine1();
	xyCrossPos2 = pos.toPoint();
	updateCrossLine2();
}


void NewPhysicsTab::updateCrossLine1()
{
	QVector<double> Result = currawImage.resultReal.at(curImageIndex);;//输入当前图片
	int imageWidth = currawImage.width;
	int imageHeight = currawImage.height;
	rawDataView->updataXYPlot(Result, imageWidth, imageHeight, xyCrossPos1.x(), xyCrossPos1.y());
	Result.clear();
}
void NewPhysicsTab::updateCrossLine2()
{
	QVector<double> Result = curMRIimage.resultReal.at(curImageIndex);//输入当前图片
	int imageWidth = curMRIimage.width;
	int imageHeight = curMRIimage.height;
	MRIdataView->updataXYPlot(Result, imageWidth, imageHeight, xyCrossPos2.x(), xyCrossPos2.y());
	Result.clear();
}

void NewPhysicsTab::on_addROIBtnClicked()
{
	rawDataView->mygv->enableSelectMode(true);
	MRIdataView->mygv->enableSelectMode(true);
}

void NewPhysicsTab::on_roiSelect(QPointF pos)
{
	int ROInum = ROIList.size();
	ROImode ROI1;
	ROI1.isChecked = true;
	ROI1.ROIorder = ROInum;
	ROI1.disp = "ROI" + QString::number(ROInum);

	//图像的数据是 curData,从0开始
	int sizeWidth = curData.widthImage;
	int sizeHeight = curData.heightImage;
	int xstart = pos.x() - ROIsizehalf;
	int xend = pos.x()+ ROIsizehalf;
	int ystart = pos.y() - ROIsizehalf;
	int yend = pos.y() + ROIsizehalf;
	xstart = (xstart < 0 ? 0 : xstart);
	xend = (xend > sizeWidth-1 ? sizeWidth-1 : xend);
	ystart = (ystart < 0 ? 0 : ystart);
	yend = (yend > sizeHeight-1 ? sizeHeight-1 : yend);

	//获取值
	double PDtemp=0,T1temp=0,T2temp=0;
	int num = 0;
	QVector<double> PDone,T1one,T2one;
	PDone = curData.PDmap.at(0);
	T1one = curData.T1map.at(0);
	T2one = curData.T2map.at(0);
	for (int iwidth = xstart; iwidth <= xend; iwidth++)
	{
		for (int iheight = ystart; iheight <= yend; iheight++)
		{
			PDtemp = PDtemp + PDone.at(iheight*sizeHeight + iwidth);
			T1temp = T1temp + T1one.at(iheight*sizeHeight + iwidth);
			T2temp = T2temp + T2one.at(iheight*sizeHeight + iwidth);
			num = num + 1;
		}
	}
	ROI1.PD = (int)(PDtemp /num*1000);//ms
	ROI1.T1 = (int)(T1temp /num*1000);
	ROI1.T2 = (int)(T2temp /num*1000);

	ROI1.xyRect = QRectF(pos.x()-2, pos.y()-2, 4, 4);
	ROI1.ROIColor = QColor(std::rand() % 245 + 10, std::rand() % 245 + 10, std::rand() % 245 + 10);//随机的颜色

	ROIList.append(ROI1);
	roiTable->updateTableView(ROIList);
	rawDataView->mygv->updateMyROI(ROIList);
	MRIdataView->mygv->updateMyROI(ROIList);
	updateCurves();
}
void NewPhysicsTab::on_TESliderChanged(int value)
{
	if(value>0 && value <=TEmax)
	{
		TEvalue = value;
	}
	phyPara->ui.TEvalueLabel->setText(QString::number(TEvalue)+"ms");
	updateCurves();
	updateMRIdataView();
}

void  NewPhysicsTab::on_TRSliderChanged(int value)
{
	if (value>0 && value <= TRmax)
	{
		TRvalue = value;
	}
	phyPara->ui.TRvalueLabel->setText(QString::number(TRvalue)+"ms");
	updateCurves();
	updateMRIdataView();
}

void NewPhysicsTab::updateCurves()
{
	SECurve->clearCurves();
	QList<QPair<QVector<double>, QVector<double>>> curveList;
	//计算SE曲线 原始的
	int ROInum = ROIList.size();
	QPair<QVector<double>, QVector<double>> curve;

	//更新曲线的时候把 T2,T1的曲线画上去
	for (int iROI = 0; iROI < ROInum; iROI++)
	{
		int PD = ROIList.at(iROI).PD;
		int T1 = ROIList.at(iROI).T1;
		int T2 = ROIList.at(iROI).T2;
		CalSECurve(PD, T1, T2, 10*T2, 10*T1, curve);
		curveList.append(curve);
		CalSECurve(PD, T1, T2, TEvalue, TRvalue, curve);
		curveList.append(curve);
	}
	SECurve->updateCurve(curveList,ROIList);
}

void NewPhysicsTab::updateMRIdataView()
{
	ParaMRI para;
	para.TE = TEvalue;
	para.TR = TRvalue;
	QList<QVector<double>> imgVector;
	CalEchoSignal(curData, para,imgVector);
	int imageWidth = curData.widthImage;
	int imageHeight = curData.heightImage;
	QPair<int, int> sizeImage(imageWidth, imageHeight);
	//第二个窗口
	curMRIimage.resultReal = imgVector;
	curMRIimage.height = imageHeight;
	curMRIimage.width = imageWidth;
	MRIdataView->mygv->updateGraphyData(imgVector, sizeImage);
	//更新标签,
	myAnnoTL.itemsStr.clear();
	myAnnoTL.pos = QPointF(0, 0);
	myAnnoTL.itemsStr.append("Name:" + curData.fileName);//文件名
	myAnnoTL.itemsStr.append("SE");
	myAnnoTL.itemsStr.append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
	myAnnoTL.itemsStr.append(QString::number(curData.widthImage) + " X "
		+ QString::number(curData.heightImage));
	myAnnoTL.itemsStr.append("TE="+QString::number(TEvalue)+"ms");
	myAnnoTL.itemsStr.append("TR=" + QString::number(TRvalue) + "ms");
	myAnnoTL.itemsStr.append("SE");
	myAnnoTL.itemsStr.append("SCALE:1 X");

	myAnnoBL.pos = QPointF(0, -20);
	myAnnoBL.itemsStr.clear();
	myAnnoBL.itemsStr.append("Position:");//0
	myAnnoBL.itemsStr.append("Value:");//1
	myAnnoBL.itemsStr.append("WL: WW:");
	myAnnoBL.itemsStr.append("");
	MRIdataView->mygv->updateAnnoData(myAnnoTL, cornerTypeEnum::TL);
	MRIdataView->mygv->updateAnnoData(myAnnoBL, cornerTypeEnum::BL);

	if (isAutoWP == 1)
	{
		MRIdataView->mygv->myAutoWP();
	}
	else
	{
		if(isGetWPWin ==true)
		{
			VectorImageWinCal(imgVector, myWin);
			isGetWPWin = false;
		}

		MRIdataView->mygv->updateMyWin(myWin);

	}
	//MRIdataView->mygv->myfitinview();

	xyCrossPos1.setX(currawImage.width / 2);
	xyCrossPos1.setY(currawImage.height / 2);

	if(currawImage.resultReal.size()>curImageIndex)
	{ 
		QVector<double > Result = currawImage.resultReal.at(curImageIndex);;//输入当前图片
		MRIdataView->updataXYPlot(Result, imageWidth, imageHeight, imageWidth / 2, imageHeight / 2);
	}
}

void NewPhysicsTab::on_autoWPcheckStateChanged(int value)
{
	isAutoWP = value;
	if (isAutoWP == false)
	{
		isGetWPWin = true;
	}
	else
	{
		isGetWPWin = false;
	}
}
NewPhysicsTab::~NewPhysicsTab()
{
}
void NewPhysicsTab::on_legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
	int curidx = 0;
	QString newName;
		if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
		{
			QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
			bool ok;
			newName = QInputDialog::getText(this, "", "Rename:", QLineEdit::Normal, plItem->plottable()->name(), &ok, windowFlags()|Qt::WindowStaysOnTopHint| Qt::FramelessWindowHint);
			if (ok)
			{
				int itemnum = legend->itemCount();
				for (int iitem = 0; iitem < itemnum; iitem++)
				{
					if (legend->item(iitem) == plItem)
					{
						curidx = iitem;
					}
				}
				ROImode ROItemp;
				ROItemp = ROIList.at(curidx);
				ROItemp.disp = newName;
				ROIList.replace(curidx, ROItemp);
				updateCurves();
				roiTable->updateTableView(ROIList);
			}	
		}
}

void NewPhysicsTab::on_jobDelectAll()
{
	ROIList.clear();
	rawDataView->mygv->updateMyROI(ROIList);
	MRIdataView->mygv->updateMyROI(ROIList);
	updateCurves();
	roiTable->updateTableView(ROIList);
}
void NewPhysicsTab::on_jobDelectItem()
{
	//获取当前选中项
	QModelIndex curindex = roiTable->currentIndex();
	if (curindex.isValid()) {
		int row = curindex.row();//是从1开始的
		if (ROIList.size() > row) {
			ROIList.removeAt(row);
		}
		else
		{
		}
	}
	rawDataView->mygv->updateMyROI(ROIList);
	MRIdataView->mygv->updateMyROI(ROIList);
	updateCurves();
	roiTable->updateTableView(ROIList);
}

void NewPhysicsTab::on_copyImage()
{
	QClipboard *clip = QApplication::clipboard();
	QSize widsize1 = rawDataView->size();
	QSize widsize2 = MRIdataView->size();
	QImage imageAll(widsize1.width()+ widsize2.width(), widsize1.height(), QImage::Format_ARGB32);
	QPainter painter(&imageAll);
	rawDataView->render(&painter,QPoint(0,0),QRect(0, 0, widsize1.width(), widsize1.height()));   //关键函数
	MRIdataView->render(&painter,QPoint(widsize2.width(),0),QRect(0, 0, widsize2.width(), widsize2.height()));   //关键函数
	clip->setImage(imageAll);
}


void NewPhysicsTab::closePatient()
{
	curData.clear();
	currawImage.resultReal.clear();
	curMRIimage.resultReal.clear();
	curImageIndex = 0;

	roiTable->clearTableView();
	ROIList.clear();
	rawDataView->mygv->clearGraphyView();
	MRIdataView->mygv->clearGraphyView();
}