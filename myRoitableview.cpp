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
	this->addAction(delectAllItemAct);  //��Ӷ�����thisָ�������ڲ���

	this->setContextMenuPolicy(Qt::ActionsContextMenu); //������
}

myRoitableview::~myRoitableview()
{
}

//ֻ������ʾ���������ݽ��д���
//table�������ʾ
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
	horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);//�Ե�0�е������ù̶����
	setColumnWidth(0, 120);//���ù̶����
	horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
	horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

	// �̶����߶Ȳ�����չ�������ֶ������߶�
	//verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader()->setDefaultSectionSize(40);
	// ����������
	setShowGrid(false);
	setGridStyle(Qt::DotLine);

	setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ģʽΪѡ����
	setSelectionMode(QAbstractItemView::SingleSelection);//��ֹѡ�����
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	//������������ɫ������ʾ
	setAlternatingRowColors(true);
}


void myRoitableview::clearTableView()
{
	myDbtableModel->clear();
	setMytable();
}