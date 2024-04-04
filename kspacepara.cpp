#include "kspacepara.h"

kspacepara::kspacepara(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.smoothchk->setChecked(false); 
	ui.smoothtype->addItems(SMOOTHTYPE);
	ui.smoothtype->setView(new QListView());
	ui.smoothsize->setValue(3);
	ui.smoothsize->setRange(1, 11);
	ui.smoothsize->setSingleStep(2);

	ui.windowchk->setChecked(true);
	ui.windowtype->addItems(WINDOWTYPE);
	ui.windowtype->setView(new QListView());
	ui.windowsize->setValue(100);
	ui.windowsize->setRange(0, 100);
	ui.windowsize->setSingleStep(1);

	ui.partialFouriertype->addItems(PFTYPE);
	ui.partialFouriertype->setView(new QListView());
	ui.partialFouriePercent->setValue(100);
	ui.partialFouriePercent->setRange(0, 100);
	ui.partialFouriePercent->setSingleStep(1);

	ui.undersamplechk->setChecked(false); 
	ui.undersampletype->addItems(USTYPE);
	ui.undersampletype->setView(new QListView());
	ui.undersamplepercent->setValue(0);
	ui.undersamplepercent->setRange(0, 100);
}

kspacepara::~kspacepara()
{
}
 
void kspacepara::getProcessPara(ParaPro &para)
{
	para.isSmooth = ui.smoothchk->isChecked();
	para.smoothtype = SMOOTHTYPE.indexOf(ui.smoothtype->currentText());
	para.smoothsize = ui.smoothsize->value();

	para.isWindow = ui.windowchk->isChecked();
	para.windowtype = WINDOWTYPE.indexOf(ui.windowtype->currentText());
	para.windowsize = ui.windowsize->value();

	para.isPF =ui.PFchk->isChecked();
 	para.PFtype = PFTYPE.indexOf(ui.partialFouriertype->currentText());
	para.PFPercent = ui.partialFouriePercent->value();

	para.isUS = ui.undersamplechk->isChecked();
	para.UStype = USTYPE.indexOf(ui.undersampletype->currentText());
	para.USPercent = ui.undersamplepercent->value();
}