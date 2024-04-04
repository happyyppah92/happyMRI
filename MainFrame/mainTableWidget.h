
#ifndef MainWidget_H
#define MainWidget_H


#include <QItemSelection>
#include <QTabWidget>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QLabel>
#include "./ViewerMain/newAcquiretab.h"
#include "./ViewerMain/newKspaceTab.h"
#include "./ViewerMain/newPhysicsTab.h"
#include "./ViewerMain/newViewertab.h"

//主窗口的主界面
class MainTableWidget : public QTabWidget
{
	Q_OBJECT

public:
	MainTableWidget(QWidget* parent = 0);

public slots:
	void tabChanged(int);

signals:
	void setupMainWindows(TabnameEnum name);

public:
	void setupTabs();
	newViewertab* ViewrTab;
	NewAquireTab *AcquireTab;
	NewKspaceTab *KSpaceTab;
	NewPhysicsTab *PhysicsTab;

private:
	void tranDataFromAcq2kspace();
	void tranDataFromAcq2physic();
};


#endif // MainWidget_H
