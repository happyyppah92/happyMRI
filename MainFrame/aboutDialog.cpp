#include "aboutDialog.h"
#include "ui_aboutdialog.h"
#include <QtDebug>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("color:white; border: 1px solid white;");
    ui->textEdit->setStyleSheet("border: 0px solid white;");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		last_mouse_position = event->globalPos();
}

void AboutDialog::mouseMoveEvent(QMouseEvent *event)
{
	if (!event->buttons().testFlag(Qt::LeftButton))
		return;
	const QPoint position = pos() + event->globalPos() - last_mouse_position; //the position of mainfrmae + (current_mouse_position - last_mouse_position)
	move(position.x(), position.y());
	last_mouse_position = event->globalPos();
}