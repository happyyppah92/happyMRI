// 采集界面
#include "newViewertab.h"
#include "global.h"

#include <QtWidgets>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTransform>

newViewertab::newViewertab(QWidget *parent)
{
	dicomGUI = new dicomGui::GUI(this);
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(dicomGUI);
	mainLayout->setMargin(0);

	this->setLayout(mainLayout);
}

void newViewertab::openfile_slot()
{
	dicomGUI->onOpenFile();
}

void newViewertab::openfolder_slot()
{
	dicomGUI->onOpenFolder();
}

void newViewertab::closepatient_slot()
{
	dicomGUI->onCloseAllPatients();
}

void newViewertab::onLayoutMenu_slot(QAction* t_action)
{
    const auto action = t_action->objectName();
    if (action == "sigle")
    {
        dicomGUI->onChangeLayout(layouts::one);
    }
    else if (action == "2Row1Bottom")
    {
        dicomGUI->onChangeLayout(layouts::twoRowOneBottom);
    }
    else if (action == "3Row1Bottom")
    {
        dicomGUI->onChangeLayout(layouts::threeRowOneBottom);
    }
    else if (action == "2Column1Right")
    {
        dicomGUI->onChangeLayout(layouts::twoColumnOneRight);
    }
    else if (action == "3Column1Right")
    {
        dicomGUI->onChangeLayout(layouts::threeColumnOneRight);
    }
}

void newViewertab::process3D_slot()
{
    dicomGUI->onCreateWidget3D();
}

void newViewertab::processMPR_slot()
{
    dicomGUI->onCreateWidgetMPR();
}

void newViewertab::processMPR3D_slot()
{
	dicomGUI->onCreateWidgetMPRand3D();
}