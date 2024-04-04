#ifndef MYSEQUENCE_H
#define MYSEQUENCE_H
#include "global.h"
#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QList>
//序列选择窗口
namespace Ui {
class mySeqDialog;
}

class mySequenceView: public QDialog
{
Q_OBJECT
public:
    mySequenceView(QWidget *parent = 0);
    QList<QVector<QString>> myseqlist;
    Ui::mySeqDialog *ui;
signals:
    void seqChange(seqTypeEnum st);
public slots:
    void on_SeqSelect(const QModelIndex index);
private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint last_mouse_position_;
    QStandardItemModel* myDbtableModel;
};
#endif // MYSEQUENCE_H
