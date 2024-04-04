#pragma once

#include <QWidget>
#include <QListView>
#include "ui_kspacepara.h"
#include "dataMode.h"

class kspacepara : public QWidget
{
	Q_OBJECT

public:
	kspacepara(QWidget *parent = Q_NULLPTR);
	~kspacepara();

	Ui::kspacepara ui;
	void getProcessPara(ParaPro &para);
};
