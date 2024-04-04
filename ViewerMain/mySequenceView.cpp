#include "mySequenceView.h"
#include "ui_mysequencediag.h"
#include <QHeaderView>
#include <QStandardItem>
#include <QtDebug>
mySequenceView::mySequenceView(QWidget *parent)
{
    Q_UNUSED(parent);
    ui =new Ui::mySeqDialog;
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("color:white; border: 1px solid white;");
    ui->tableView->setStyleSheet("color:white; border: 0px solid white;");

    myDbtableModel = new QStandardItemModel();
    ui->tableView->setModel(myDbtableModel);

    QStringList table_h_headers;
    table_h_headers <<"Sequence" << "Description";
    myDbtableModel->setHorizontalHeaderLabels(table_h_headers);

    QStringList table_v_headers;
    myDbtableModel->setVerticalHeaderLabels(table_v_headers);

    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableView->verticalHeader()->setDefaultAlignment(Qt::AlignVCenter);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);//对第0列单独设置固定宽度
    ui->tableView->setColumnWidth(0, 100);//设置固定宽度
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    // 固定表格高度不可扩展，不可手动调整高度
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setDefaultSectionSize(40);
    // 隐藏网格线
    ui->tableView->setShowGrid(false);
    ui->tableView->setGridStyle(Qt::DotLine);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setSelectionMode ( QAbstractItemView::SingleSelection);//禁止选择多行
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    //设置相邻行颜色交替显示
    ui->tableView->setAlternatingRowColors(true);

    // 填充表格
    QVector<QString> seqItem;
    seqItem.append("");
    seqItem.append("");
    seqItem.replace(0,"SE");
    seqItem.replace(1,"自旋回波序列");
    myseqlist.append(seqItem);

    seqItem.replace(0,"IR");
    seqItem.replace(1,"反转恢复序列");
    myseqlist.append(seqItem);

//    seqItem.replace(0,"GE");
//    seqItem.replace(1,"梯度回波序列");
//    myseqlist.append(seqItem);

    int numItem=myseqlist.size();
    qDebug()<<numItem;
    for(int iitem=0;iitem<numItem;iitem++){
        myDbtableModel->setItem(iitem,0,new QStandardItem(myseqlist.at(iitem).at(0)));
        myDbtableModel->setItem(iitem,1,new QStandardItem(myseqlist.at(iitem).at(1)));
    }
    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(on_SeqSelect(const QModelIndex &)));
	seqItem.clear();
}

void mySequenceView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        last_mouse_position_ = event->globalPos();
}

void mySequenceView::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::LeftButton))
        return;
    const QPoint position = pos() + event->globalPos() - last_mouse_position_; //the position of mainfrmae + (current_mouse_position - last_mouse_position)
    move(position.x(), position.y());
    last_mouse_position_ = event->globalPos();
}

void mySequenceView::on_SeqSelect(const QModelIndex index)
{
    if(index.isValid()){
        int row=index.row();
        //row是从0开始的
        if(myseqlist.size()>row){
            qDebug()<< myseqlist.at(row).at(0);
            if(myseqlist.at(row).at(0).compare("SE")==0){
                emit seqChange(seqTypeEnum::SE);
            }else if(myseqlist.at(row).at(0).compare("IR")==0){
                emit seqChange(seqTypeEnum::IR);
            }else if(myseqlist.at(row).at(0).compare("GE")==0){
                emit seqChange(seqTypeEnum::GE);
            }
        }
        this->close();
    }
}
