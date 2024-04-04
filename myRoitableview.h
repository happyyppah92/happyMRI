#pragma once

#include <QTableView>
#include <QStandardItemModel>
#include <QList>
#include <QAction>
#include "dataMode.h"

class myRoitableview : public QTableView
{
public:
	myRoitableview(QWidget *parent=0);
	~myRoitableview();

	void setMytable();
	void updateTableView(QList<ROImode> myRoilist);//
	void clearTableView();

	QAction *delectItemAct;//´°¿ÚÍ¼Ïñ¸´ÖÆ
	QAction *delectAllItemAct;
private:
	QStandardItemModel* myDbtableModel;
};
