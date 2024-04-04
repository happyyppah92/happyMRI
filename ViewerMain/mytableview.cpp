#include "mytableview.h"

#include <QHeaderView>
#include <QString>
myTableView::myTableView(QWidget *parent)
{
	Q_UNUSED(parent)
    myDbtableModel = new QStandardItemModel();
    setModel(myDbtableModel);

    setMytable();

    delectItemAct =new QAction;
    delectItemAct->setText("Delect Item");

    delectAllItemAct=new QAction;
    delectAllItemAct->setText("Delect All");

    this->addAction(delectItemAct);
    this->addAction(delectAllItemAct);  //添加动作，this指定父窗口部件

    this->setContextMenuPolicy(Qt::ActionsContextMenu); //设置上
}

//只用于显示，不对数据进行处理
//table里面的显示
void myTableView::addNewJob(QList<myJobItem> myjoblist)
{
    myDbtableModel->clear();
    setMytable();
    int numjob=myjoblist.size();
    for(int ijob=0;ijob<numjob;ijob++){
       myDbtableModel->setItem(ijob,0,new QStandardItem(myjoblist.at(ijob).getJobHead()));
       myDbtableModel->setItem(ijob,1,new QStandardItem(myjoblist.at(ijob).getJobSeqdescrip()));
       myDbtableModel->setItem(ijob,2,new QStandardItem(myjoblist.at(ijob).getNumImage()));
    }
}

myTableView::~myTableView()
{

}

void myTableView::setMytable()
{
    QStringList table_h_headers;
    table_h_headers <<"" << "Description" << "Data";
    myDbtableModel->setHorizontalHeaderLabels(table_h_headers);

    QStringList table_v_headers;
    table_v_headers << "1";
    myDbtableModel->setVerticalHeaderLabels(table_v_headers);

    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    verticalHeader()->setDefaultAlignment(Qt::AlignVCenter);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);//对第0列单独设置固定宽度
    setColumnWidth(0, 60);//设置固定宽度
    horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);//对第0列单独设置固定宽度
    setColumnWidth(2, 60);//设置固定宽度
    // 固定表格高度不可扩展，不可手动调整高度
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(40);
    // 隐藏网格线
    setShowGrid(false);
    setGridStyle(Qt::DotLine);

    setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    setSelectionMode ( QAbstractItemView::SingleSelection);//禁止选择多行
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置相邻行颜色交替显示
    setAlternatingRowColors(true);
}

void myTableView::closePatient()
{
myDbtableModel->clear();
setMytable();
}


void myTableView::delectAll()
{
myDbtableModel->clear();
setMytable();
}

