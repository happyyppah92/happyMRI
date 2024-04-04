#ifndef NewVIEWERTab_H
#define NewVIEWERTab_H

#include "global.h"
#include "gui.h"
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QTableView>
#include <QTabWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

class newViewertab : public QWidget
{
    Q_OBJECT

public:
	newViewertab(QWidget *parent = 0);
	dicomGui::GUI *dicomGUI;
public slots:
	void openfile_slot();
	void openfolder_slot();
	void closepatient_slot();
	void onLayoutMenu_slot(QAction* t_action);
	void process3D_slot();
	void processMPR_slot();
	void processMPR3D_slot();
	
private slots:

signals:
	   void updateStatus(QString, msgClassEnum);

private:
};

#endif
