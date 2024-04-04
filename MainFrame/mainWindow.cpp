// 主界面 带菜单
#include "mainWindow.h"
#include "./MainFrame/aboutDialog.h"
#include "myfileopen.h"
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QtMath>
#include <QVector>
#include <algorithm>
#include <QStatusBar>
#include <QLabel>
#include <QString>
#include <qhboxlayout>


//APP->MainWindow->MainWidget->tabs
MainWindow::MainWindow(QWidget* parent)
{
    Q_UNUSED(parent)
    mainTableWidget = new MainTableWidget;//中间的主要界面
    setCentralWidget(mainTableWidget);
 
    setWindowTitle(tr(APPname));
    setWindowIcon(QIcon(":/icon/Resources/mri.ico"));
    statusLabel = new QLabel("");
    statusLabel->setMinimumSize(100, 20); // 设置标签最小大小
    statusBar()->addWidget(statusLabel);
    updateStatus("Ready", msgClassEnum::GREEN);
	normalFlag = this->windowFlags();
	setWindowFlags(Qt::FramelessWindowHint);
	currentFramelessFlag = true;

	menuWidget = new QWidget();
	menuBtn = new QPushButton;
	menuBtn->setIcon(QIcon(":/windows"));
	menuBtn->setProperty("class", "1");
	menuLabel=new QLabel(APPname);
	menuLabel->setProperty("class", "1");
    menuWidgetLayout = new QHBoxLayout(menuWidget);
	menuWidgetLayout->setMargin(0);
	menuWidget->setLayout(menuWidgetLayout);
	mainMenubar = new QMenuBar;
	menuWidgetLayout->addWidget(mainMenubar);
	menuWidgetLayout->addWidget(menuLabel);
	QSpacerItem *hspacer = new QSpacerItem(100, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);
	menuWidgetLayout->addSpacerItem(hspacer);
	menuWidgetLayout->addWidget(menuBtn);
	setMenuWidget(menuWidget);

	createACQMenus();
	creatAboutMenus();
     //全局的信号--需要传到主界面的信号
	connect(this->menuBtn, &QPushButton::clicked, this, &MainWindow::menuBtnClick_slot);
    connect(mainTableWidget->ViewrTab, SIGNAL(updateStatus(QString, msgClassEnum)), this, SLOT(updateStatus(QString, msgClassEnum)));
    connect(mainTableWidget, SIGNAL(setupMainWindows(TabnameEnum)), this, SLOT(setupMainWindows(TabnameEnum)));
}

void MainWindow::setupMainWindows(TabnameEnum name)
{
    if (TabnameEnum::VIEWER== name)
    {
        createViewerMenus();//
    }
    else//(TabnameEnum::ACQ == name)
    {
        createACQMenus();
    }

	creatAboutMenus();
}
void MainWindow::createViewerMenus()
{
	mainMenubar->clear();
    //file 菜单
    fileMenu = mainMenubar->addMenu(tr("&Patient"));

    openfileAct = new QAction(tr("Open &File..."), this);
    openfileAct->setIcon(QIcon(":/openfile"));
	openfileAct->setShortcut(tr("Ctrl+F"));
    fileMenu->addAction(openfileAct);

    openfolderAct = new QAction(tr("Open Fol&der..."), this);
    openfolderAct->setIcon(QIcon(":/openfolder"));
	openfolderAct->setShortcut(tr("Ctrl+D"));
    fileMenu->addAction(openfolderAct);

    closepatientAct = new QAction(tr("&Close Patient"), this);
    closepatientAct->setIcon(QIcon(":/closeall"));
	closepatientAct->setShortcut(tr("Ctrl+R"));
    fileMenu->addAction(closepatientAct);

    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setIcon(QIcon(":/exit"));
	exitAct->setShortcut(tr("Ctrl+X"));
    fileMenu->addAction(exitAct);
   
    layoutmenu = mainMenubar->addMenu(tr("&Layout"));
    lay1Act = new QAction(tr("sigle"), this);
    lay1Act->setIcon(QIcon(":/res/0.png"));
    lay1Act->setObjectName("sigle");
	lay1Act->setShortcut(tr("Shift+1"));
    layoutmenu->addAction(lay1Act);

    lay3Act = new QAction(tr("2Row1Bottom"), this);
    lay3Act->setIcon(QIcon(":/res/2.png"));
    lay3Act->setObjectName("2Row1Bottom");
	lay3Act->setShortcut(tr("Shift+2"));
    layoutmenu->addAction(lay3Act);

    lay4Act = new QAction(tr("3Row1Bottom"), this);
    lay4Act->setIcon(QIcon(":/res/3.png"));
    lay4Act->setObjectName("3Row1Bottom");
	lay4Act->setShortcut(tr("Shift+3"));
    layoutmenu->addAction(lay4Act);

    lay2Act = new QAction(tr("2Column1Right"), this);
    lay2Act->setIcon(QIcon(":/res/1.png"));
    lay2Act->setObjectName("2Column1Right");
	lay2Act->setShortcut(tr("Shift+4"));
    layoutmenu->addAction(lay2Act);

    lay5Act = new QAction(tr("3Column1Right"), this);
    lay5Act->setIcon(QIcon(":/res/4.png"));
    lay5Act->setObjectName("3Column1Right");
	lay5Act->setShortcut(tr("Shift+5"));
    layoutmenu->addAction(lay5Act);

    processMenu = mainMenubar->addMenu(tr("&Process"));
    process3DAct = new QAction(tr("3D"), this);
    process3DAct->setIcon(QIcon(":/3D"));
	process3DAct->setShortcut(tr("Ctrl+3"));
    processMenu->addAction(process3DAct);

    processMPRAct = new QAction(tr("MPR"), this);
    processMPRAct->setIcon(QIcon(":/mpr2"));
	processMPRAct->setShortcut(tr("Ctrl+4"));
    processMenu->addAction(processMPRAct);

	processMPR3DAct = new QAction(tr("MPR3D"), this);
	processMPR3DAct->setIcon(QIcon(":/mpr3d"));
	processMPR3DAct->setShortcut(tr("Ctrl+5"));
	processMenu->addAction(processMPR3DAct);

   
    connect(openfileAct, &QAction::triggered, mainTableWidget->ViewrTab, &newViewertab::openfile_slot);
    connect(openfolderAct, &QAction::triggered, mainTableWidget->ViewrTab, &newViewertab::openfolder_slot);
    connect(closepatientAct, &QAction::triggered, mainTableWidget->ViewrTab, &newViewertab::closepatient_slot);
    connect(layoutmenu, &QMenu::triggered, mainTableWidget->ViewrTab, &newViewertab::onLayoutMenu_slot);

    connect(process3DAct, &QAction::triggered, mainTableWidget->ViewrTab, &newViewertab::process3D_slot);
    connect(processMPRAct, &QAction::triggered, mainTableWidget->ViewrTab, &newViewertab::processMPR_slot);
	connect(processMPR3DAct, &QAction::triggered, mainTableWidget->ViewrTab, &newViewertab::processMPR3D_slot);

    connect(exitAct, &QAction::triggered, this, &MainWindow::close);
}


void MainWindow::createACQMenus()
{
	mainMenubar->clear();
	//file 菜单
	fileMenu = mainMenubar->addMenu(tr("&Patient"));
	newpatientAct = new QAction(tr("&New Patient..."), this);
	fileMenu->addAction(newpatientAct);
	newpatientAct->setShortcut(tr("Ctrl+N"));
	connect(newpatientAct, &QAction::triggered, this, &MainWindow::newPatient);


	closepatientAct = new QAction(tr("&Close Patient..."), this);
	fileMenu->addAction(closepatientAct);
	closepatientAct->setShortcut(tr("Ctrl+R"));
	connect(closepatientAct, &QAction::triggered, this, &MainWindow::closePatient);


	fileMenu->addSeparator();
	exitAct = new QAction(tr("E&xit"), this);
	fileMenu->addAction(exitAct);
	exitAct->setShortcut(tr("Ctrl+X"));
	connect(exitAct, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::creatAboutMenus()
{
	//help 菜单
	helpMenu = mainMenubar->addMenu(tr("&Help"));
	manualAct = new QAction(tr("&Manual"), this);
	manualAct->setIcon(QIcon(":/manual"));
	manualAct->setShortcut(tr("Ctrl+M"));
	helpMenu->addAction(manualAct);

	helpMenu->addSeparator();
	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setIcon(QIcon(":/about"));
	aboutAct->setShortcut(tr("Ctrl+A"));
	helpMenu->addAction(aboutAct);

	connect(manualAct, &QAction::triggered, this, &MainWindow::about);
	connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

}
void MainWindow::close()
{
    QWidget::close();
}
void MainWindow::manual()
{

}

void MainWindow::about()
{
    AboutDialog d(this);
    d.exec();
}

void MainWindow::updateStatus(QString msg, msgClassEnum msgClass)
{

    statusLabel->setText(msg);
    if (msgClass == msgClassEnum::GREEN)
    {
        statusLabel->setStyleSheet("QLabel{color:rgb(0,255,0);}");  //设置样式表
    }
    else if (msgClass == msgClassEnum::NORMAL)
    {
        statusLabel->setStyleSheet("QLabel{color:rgb(255,255,255);}");  //设置样式表
    }
    else
    {
        statusLabel->setStyleSheet("QLabel{color:rgb(255,0,0);}");  //设置样式表
    }
}
void MainWindow::menuBtnClick_slot()
{
	if (currentFramelessFlag == true)
	{
		setWindowFlags(normalFlag);
		currentFramelessFlag = false;
		menuBtn->setIcon(QIcon(":/frameless"));
		menuLabel->setText("");
		show();
	}
	else
	{
		setWindowFlags(Qt::FramelessWindowHint);
		currentFramelessFlag = true;
		menuBtn->setIcon(QIcon(":/windows"));
		menuLabel->setText(APPname);
		show();
	}
}

//导入数据
void MainWindow::newPatient()
{
	//打开数据
	int isOK;
	myFileOpen myfileopen;
	QString fileName = QFileDialog().getOpenFileName(
		this,
		tr("New Patient"),
		"",
		tr("Patient file(*.mri *.data)"));
	//文件名为空
	if (fileName.isEmpty()) {
		QMessageBox::warning(this, "Warning!", "Failed to Open Patient!");
	}
	else {
		currentFile.setFileName(fileName);
		if (!currentFile.open(QIODevice::ReadOnly))
		{
			updateStatus("File Open:FAILL", msgClassEnum::RED);
		}
		else
		{
			closePatient();
			//读数据currentData
			isOK = myfileopen.rawPatientFileOpen(currentFile, currentData);
			if (isOK == statuOK)
			{

				updateStatus("Open a new Patient:OK", msgClassEnum::GREEN);
				//发送数据给子窗口
				mainTableWidget->AcquireTab->updateData(currentData);
				if (currentData.dataType == dataTypeEnum::MAP2D || currentData.dataType == dataTypeEnum::MAP3D) {
					mainTableWidget->AcquireTab->newBtn->setEnabled(true);
				}
			}
			else
			{
				updateStatus("File Read:FAILL", msgClassEnum::RED);
			}
		}
	}
}

void MainWindow::closePatient()
{
	currentData.clear();
	mainTableWidget->AcquireTab->closepatient();
	mainTableWidget->PhysicsTab->closePatient();
	mainTableWidget->KSpaceTab->closePatient();
	updateStatus("Close Patient", msgClassEnum::NORMAL);
}