#pragma once

#include <QWidget>
#include "ui_physicpara.h"

class physicpara : public QWidget
{
	Q_OBJECT

public:
	physicpara(QWidget *parent = Q_NULLPTR);
	~physicpara();
	Ui::physicpara ui;

private:
};
