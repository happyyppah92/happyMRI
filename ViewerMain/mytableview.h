#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H
#include "myJobItem.h"
#include <global.h>
#include <QTableView>
#include <QStandardItemModel>
#include <QList>
#include <QAction>

//Job table的显示窗口
class myTableView : public QTableView
{
public:
    myTableView(QWidget *parent = 0);
    ~myTableView() override;
    void setMytable();
    void addNewJob(QList<myJobItem> myjoblist);
    void closePatient();
    void delectAll();
  QAction *delectItemAct;//窗口图像复制
  QAction *delectAllItemAct;

private:
    QStandardItemModel* myDbtableModel;
};

#endif // MYTABLEVIEW_H
