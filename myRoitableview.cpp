#include "myRoitableview.h"
#include <QHeaderView>
#include <QString>

myRoitableview::myRoitableview(QWidget *parent)
{
	Q_UNUSED(parent);
	myDbtableModel = new QStandardItemModel();
	setModel(myDbtableModel);
	setMytable();

	delectItemAct = new QAction;
	delectItemAct->setText("Delect Item");

	delectAllItemAct = new QAction;
	delectAllItemAct->setText("Delect All");

	this->addAction(delectItemAct);
	this->addAction(delectAllItemAct);  //添加动作，this指定父窗口部件

	this->setContextMenuPolicy(Qt::ActionsContextMenu); //设置上
}

myRoitableview::~myRoitableview()
{
}

//只用于显示，不对数据进行处理
//table里面的显示
void myRoitableview::updateTableView(QList<ROImode> myRoilist)
{
	myDbtableModel->clear();
	setMytable();
	int numroi = myRoilist.size();
	for (int iroi = 0; iroi<numroi; iroi++) {
		myDbtableModel->setItem(iroi, 0, new QStandardItem(myRoilist.at(iroi).disp));
		myDbtableModel->setItem(iroi, 1, new QStandardItem(QString::number(myRoilist.at(iroi).PD)));
		myDbtableModel->setItem(iroi, 2, new QStandardItem(QString::number(myRoilist.at(iroi).T1)));
		myDbtableModel->setItem(iroi, 3, new QStandardItem(QString::number(myRoilist.at(iroi).T2)));
	}
}

void myRoitableview::setMytable()
{
	QStringList table_h_headers;
	table_h_headers << "Description" << "PD" << "T1" << "T2";
	myDbtableModel->setHorizontalHeaderLabels(table_h_headers);

	QStringList table_v_headers;
	table_v_headers << "1";
	myDbtableModel->setVerticalHeaderLabels(table_v_headers);

	horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	verticalHeader()->setDefaultAlignment(Qt::AlignVCenter);
	horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);//对第0列单独设置固定宽度
	setColumnWidth(0, 120);//设置固定宽度
	horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
	horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

	// 固定表格高度不可扩展，不可手动调整高度
	//verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader()->setDefaultSectionSize(40);
	// 隐藏网格线
	setShowGrid(false);
	setGridStyle(Qt::DotLine);

	setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
	setSelectionMode(QAbstractItemView::SingleSelection);//禁止选择多行
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置相邻行颜色交替显示
	setAlternatingRowColors(true);
}


void myRoitableview::clearTableView()
{
	myDbtableModel->clear();
	setMytable();
}