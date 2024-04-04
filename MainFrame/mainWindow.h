
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainTableWidget.h"
#include "global.h"

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QStatusBar>

//主窗口
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = 0);
	QFile currentFile;

	dataMode currentData;//当前的数据

private slots:

	void close();
	void manual();
	void about();
	void updateStatus(QString msg, msgClassEnum msgclass);
	void setupMainWindows(TabnameEnum name);
	void menuBtnClick_slot();
	void newPatient();
	void closePatient();



private:
	void createViewerMenus();
	void createACQMenus();
	void creatAboutMenus();
	MainTableWidget* mainTableWidget;
	QLabel* statusLabel;
	QPoint last_mouse_position;
	QPushButton *menuBtn;
	QLabel *menuLabel;
	Qt::WindowFlags normalFlag;
	bool currentFramelessFlag;

	QMenuBar *mainMenubar;
	QWidget* menuWidget;
	QHBoxLayout* menuWidgetLayout;
	QMenu *fileMenu;
	QMenu *layoutmenu;
	QMenu* processMenu;
	QMenu *helpMenu;
	QAction* openfileAct;
	QAction* openfolderAct;
	QAction* closepatientAct;
	QAction* exitAct;
	QAction* newpatientAct;

	QAction* lay1Act;
	QAction* lay3Act;
	QAction* lay4Act;
	QAction* lay2Act;
	QAction* lay5Act;

	QAction* process3DAct;
	QAction* processMPRAct;
	QAction* processMPR3DAct;

	QAction* manualAct;
	QAction* aboutAct;
};

#endif // MAINWINDOW_H
