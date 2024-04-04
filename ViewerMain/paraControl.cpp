//与参数有关的代码
#include "paraControl.h"

#include <QString>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QDomDocument>
#include <QMap>
#include <QPair>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

paraControl::paraControl(QTabWidget *parent)
{
    parentW=parent;
    m_mapControl["line_edit"]=paraTypeEnum::TYPE_LINEEDIT;
    m_mapControl["combo_box"]=paraTypeEnum::TYPE_COMBOBOX;
    m_mapControl["check_box"]=paraTypeEnum::TYPE_CHECKBOX;
    m_mapControl["spin_box"]=paraTypeEnum::TYPE_SPINBOX;
    m_mapControl["tabwidget"]=paraTypeEnum::TYPE_TABWIDGET;
    m_mapControl["label"]=paraTypeEnum::TYPE_LABEL;
    curlayout=new QGridLayout;
}

void paraControl::GetSEPara(ParaMRI &mypara)
{
	QString value;
	GetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TEValue", value);
	mypara.TE = value.toDouble();

	GetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TRValue", value);
	mypara.TR = value.toDouble();

	GetSingleValue(paraTypeEnum::TYPE_LINEEDIT, "Description", value);
	mypara.description = value;
}

void paraControl::GetIRPara(ParaMRI &mypara)
{
	QString value;
	GetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TEValue", value);
	mypara.TE = value.toDouble();
	GetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TRValue", value);
	mypara.TR = value.toDouble();
	GetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TIValue", value);
	mypara.TI = value.toDouble();
	GetSingleValue(paraTypeEnum::TYPE_LINEEDIT, "description", value);
	mypara.description = value;
}

void paraControl::GetGEPara(ParaMRI &mypara)
{
	QString value;
	GetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TEValue", value);
	mypara.TE = value.toDouble();
	GetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TRValue", value);
	mypara.TR = value.toDouble();
}

void paraControl::CreateUIFromXml(QString strXmlFile)
{
    //清空状态
    curTabnum=0;
    paraList.clear();
    m_para.clear();

    //分析xml文件
    AnalyzeParamsXml(strXmlFile, myList);
    qDebug()<< "sizemyList"<< myList.m_mapList.size();
    for (auto it = myList.m_mapList.begin(); it != myList.m_mapList.end(); it++)
    {
        //
        int tabPos=it->second.m_simp["tabPosition"].toInt();
        if(tabPos<mygridlayout.size()){
            curlayout=mygridlayout.at(tabPos);
        }

        curlayout->setColumnMinimumWidth(0,60);
        curlayout->setColumnMinimumWidth(1,100);
        //根据控件的类型选择
        switch (m_mapControl[it->second.strType])
        {
        case paraTypeEnum::TYPE_TABWIDGET:
            InitTabWidget(it->second);
            qDebug()<<"InitTabWidget";
            break;
        case paraTypeEnum::TYPE_LINEEDIT:
            CreateLineEdit(it->second);
            break;
        case paraTypeEnum::TYPE_SPINBOX:
            CreateSpinBox(it->second);
            break;
        case paraTypeEnum::TYPE_COMBOBOX:
            CreateCombox(it->second);
            break;
        case paraTypeEnum::TYPE_CHECKBOX:
            CreateCheckBox(it->second);
            break;
        case paraTypeEnum::TYPE_LABEL:
            CreateLabel(it->second);
            break;
        default:
            break;
        }
    }

}
void paraControl::InitTabWidget(WidgetELEM &ele)
{
    for(int iw=0;iw<mywidget.size();++iw){
        qDebug()<< mywidget.at(iw);
        delete mywidget.at(iw);
    }
    mywidget.clear();
    mygridlayout.clear();
    parentW->clear();

    int tabNum= ele.m_simp["num"].toInt();
    QVector<QString> tabString;
    for(int itab=0;itab<tabNum;++itab){
        tabString.append(ele.m_simpRange["tab"+QString::number(itab)]);
    }
    for(int itab=0;itab<tabNum;++itab){
        QWidget *w=new QWidget;
        QGridLayout *l=new QGridLayout;
        mywidget.append(w);
        mygridlayout.append(l);
        w->setLayout(l);
        QString str=tabString.at(itab);
        parentW->addTab(mywidget.at(itab),str);
    }

    //设置tab的属性
    parentW->setCurrentIndex(ele.m_simp["default"].toInt());
    parentW->setToolTip(ele.m_simp["tips"]);
    parentW->setMaximumWidth(ele.m_simp["width"].toInt());
    parentW->setMaximumHeight(ele.m_simp["height"].toInt());

	tabString.clear();
}


void paraControl::GetSingleValue(paraTypeEnum type, QString name,QString &value)
{
    if(type==paraTypeEnum::TYPE_LINEEDIT){
        QLineEdit *findptr= parentW-> findChild<QLineEdit *>(name);
        if(findptr!=nullptr){
            value=findptr->text();
            return;
        }
    }else if (type==paraTypeEnum::TYPE_COMBOBOX) {
        QComboBox *findptr= parentW-> findChild<QComboBox *>(name);
        if(findptr!=nullptr){
            value=findptr->currentText();
            return;
        }
    }else if (type==paraTypeEnum::TYPE_CHECKBOX) {
        QCheckBox *findptr= parentW-> findChild<QCheckBox *>(name);
        if(findptr!=nullptr){
            int valuei=findptr->checkState();
            value=QString::number(valuei);
            return;
        }
    }else if (type==paraTypeEnum::TYPE_SPINBOX) {
        QSpinBox *findptr= parentW-> findChild<QSpinBox *>(name);
        if(findptr!=nullptr){
            int valuei=findptr->value();
            value=QString::number(valuei);
            return;
        }

    }
    value="ERROR";
}

void paraControl::AnalyzeParamsXml(QString strFile, WidEmList &myList)
{
    myList.m_mapList.clear();
    QFile qFile(strFile);
    qDebug()<< qFile.fileName();
    if (!qFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    QString str = qFile.readAll();
    QDomDocument XMLdoc;
    bool bLoad = XMLdoc.setContent(str);
    if (!bLoad)
    {
        return;
    }
    QDomElement root = XMLdoc.documentElement();  //获取根节点元素 args
    QDomElement configElement = root.firstChildElement();//根节点的子节点  em

    while (!configElement.isNull())
    {
        QString strParmName = "";
        QString strControlType = "";
        WidgetELEM tEM;//控件信息
        //myList->WidEleList->WidElement

        QDomElement pElementNode = configElement.firstChildElement();
        while (!pElementNode.isNull())
        {
            QString value = pElementNode.text();
            QString text = pElementNode.nodeName();
            if (text == "name")
            {
                strParmName = value;
            }
            if (text == "widget")
            {
                strControlType = value;
            }
            if (text == "scope")
            {
                QMap<QString, QString> mapL;
                QDomElement pElementNodeChild = pElementNode.firstChildElement(); //ElementMeta
                while (!pElementNodeChild.isNull())
                {
                    QString valueChild = pElementNodeChild.text();
                    QString textChlid = pElementNodeChild.nodeName();
                    mapL[textChlid] = valueChild;
                    pElementNodeChild = pElementNodeChild.nextSiblingElement();
                }
                tEM.m_simpRange = mapL;
            }
            tEM.m_simp[text] = value;
            pElementNode = pElementNode.nextSiblingElement(); //查找下一个兄弟节点的指针
        }

        configElement = configElement.nextSiblingElement();
        tEM.strType = strControlType;

        myList.m_mapList.push_back(QPair<QString,WidgetELEM>( strParmName, tEM));
    }

}

void paraControl::CreateLineEdit(WidgetELEM &ele)
{
    QString nameLineEdit= ele.m_simp["name"];
    int indexH=ele.m_simp["indexH"].toInt();
    int indexW=ele.m_simp["indexW"].toInt();
    QString nameLabel=ele.m_simp["labelname"];
    QString unitLabel=ele.m_simp["unitname"];
    QString defaultValue=ele.m_simp["default"];
    bool editable=ele.m_simp["editable"].toInt();
    QString tips=ele.m_simp["tips"];
    QString dataType=ele.m_simp["datatype"];
    QString maxValue=ele.m_simp["maxvalue"];
    QString minValue=ele.m_simp["minvalue"];
    int width=ele.m_simp["width"].toInt();
    int height=ele.m_simp["height"].toInt();
    QLineEdit* pEdit =new QLineEdit(parentW);
    pEdit->setEnabled(editable);
    pEdit->setObjectName(nameLineEdit);
    //设置默认值
    pEdit->setText(defaultValue);
    //设置取值范围
    pEdit->setToolTip(tips);
    QLabel* pLabelname = new QLabel(parentW);
    pLabelname->setText(nameLabel);
    QLabel* pLabelunit = new QLabel(parentW);
    pLabelunit->setText(unitLabel);
    pEdit->setFixedWidth(width);
    pEdit->setFixedHeight(height);
    if(indexW>=100){
        curlayout->addWidget(pLabelname,indexH,0);
        curlayout->addWidget(pEdit,indexH,1,1,4);
        curlayout->addWidget(pLabelunit,indexH,indexW+2);
    }else {

        curlayout->addWidget(pLabelname,indexH,indexW+0);
        curlayout->addWidget(pEdit,indexH,indexW+1);
        curlayout->addWidget(pLabelunit,indexH,indexW+2);
    }
    curlayout->setAlignment(pLabelname,Qt::AlignRight);
    curlayout->setAlignment(pEdit,Qt::AlignLeft);
    curlayout->setAlignment(pLabelunit,Qt::AlignLeft);


}

void paraControl::CreateCombox(WidgetELEM &ele)
{
    QString nameCombox= ele.m_simp["name"];
    int indexH=ele.m_simp["indexH"].toInt();
    int indexW=ele.m_simp["indexW"].toInt();
    QString nameLabel=ele.m_simp["labelname"];
    QString unitLabel=ele.m_simp["unitname"];
    int defaultValue=ele.m_simp["default"].toInt();
    bool editable=ele.m_simp["editable"].toInt();
    QString tips=ele.m_simp["tips"];
    QString dataType=ele.m_simp["datatype"];
    int width=ele.m_simp["width"].toInt();
    int height=ele.m_simp["height"].toInt();

    QComboBox * pCom =new QComboBox(parentW);
	pCom->setView(new QListView());
    pCom->setFixedWidth(width);
    pCom->setFixedHeight(height);
    pCom->setEnabled(editable);
    pCom->setObjectName(nameCombox);
    //设置默认值
    int comSize=ele.m_simpRange.size();
    for(int icom=0;icom<comSize;++icom){
        pCom->addItem(ele.m_simpRange["item"+QString::number(icom)]);
    }

    pCom->setCurrentIndex(defaultValue);
    //设置取值范围
    pCom->setToolTip(tips);
    QLabel* pLabelname = new QLabel(parentW);
    pLabelname->setText(nameLabel);

    QLabel* pLabelunit = new QLabel(parentW);
    pLabelunit->setText(unitLabel);

    curlayout->addWidget(pLabelname,indexH,indexW+0);
    curlayout->addWidget(pCom,indexH,indexW+1);
    curlayout->addWidget(pLabelunit,indexH,indexW+2);

    curlayout->setAlignment(pLabelname,Qt::AlignRight);
    curlayout->setAlignment(pCom,Qt::AlignLeft);
    curlayout->setAlignment(pLabelunit,Qt::AlignLeft);
}

void paraControl::CreateCheckBox(WidgetELEM &ele)
{
    QString nameCheck= ele.m_simp["name"];
    int indexH=ele.m_simp["indexH"].toInt();
    int indexW=ele.m_simp["indexW"].toInt();
    QString nameLabel=ele.m_simp["labelname"];
    QString unitLabel=ele.m_simp["unitname"];
    int defaultValue=ele.m_simp["default"].toInt();
    bool editable=ele.m_simp["editable"].toInt();
    QString tips=ele.m_simp["tips"];
    QString dataType=ele.m_simp["datatype"];
    int width=ele.m_simp["width"].toInt();
    int height=ele.m_simp["height"].toInt();

    QCheckBox * pCheck =new QCheckBox(parentW);
    pCheck->setFixedWidth(width);
    pCheck->setFixedHeight(height);
    pCheck->setEnabled(editable);
    pCheck->setObjectName(nameCheck);
    //设置默认值
    if(dataType.compare("Tristate")==0){
        pCheck->setTristate(1);
    }else {
        pCheck->setTristate(0);
    }

    if(defaultValue==1){
        pCheck->setCheckState(Qt::Checked);
    }else {
        pCheck->setCheckState(Qt::Unchecked);
    }

    pCheck->setToolTip(tips);
    QLabel* pLabelname = new QLabel(parentW);
    pLabelname->setText(nameLabel);

    QLabel* pLabelunit = new QLabel(parentW);
    pLabelunit->setText(unitLabel);

    curlayout->addWidget(pLabelname,indexH,indexW+0);
    curlayout->addWidget(pCheck,indexH,indexW+1);
    curlayout->addWidget(pLabelunit,indexH,indexW+2);

    curlayout->setAlignment(pLabelname,Qt::AlignRight);
    curlayout->setAlignment(pCheck,Qt::AlignLeft);
    curlayout->setAlignment(pLabelunit,Qt::AlignLeft);
}

void paraControl::SetSingleValue(paraTypeEnum type, QString name, QString value)
{
	if (type == paraTypeEnum::TYPE_LINEEDIT) {
		QLineEdit *findptr = parentW->findChild<QLineEdit *>(name);
		if (findptr != nullptr) {
			findptr->setText(value);
			return;
		}
	}
	else if (type == paraTypeEnum::TYPE_COMBOBOX) {
		QComboBox *findptr = parentW->findChild<QComboBox *>(name);
		if (findptr != nullptr) {
			findptr->setCurrentText(value);
			return;
		}
	}
	else if (type == paraTypeEnum::TYPE_CHECKBOX) {
		QCheckBox *findptr = parentW->findChild<QCheckBox *>(name);
		if (findptr != nullptr) {
			int valuei = value.toInt();	
			findptr->setCheckState(Qt::CheckState(valuei));
			
			return;
		}
	}
	else if (type == paraTypeEnum::TYPE_SPINBOX) {
		QSpinBox *findptr = parentW->findChild<QSpinBox *>(name);
		if (findptr != nullptr) {
			int valuei = value.toInt();
			findptr->setValue(valuei);
			return;
		}

	}
}

void paraControl::CreateSpinBox(WidgetELEM &ele)
{
    QString nameSpin= ele.m_simp["name"];
    int indexH=ele.m_simp["indexH"].toInt();
    int indexW=ele.m_simp["indexW"].toInt();
    QString nameLabel=ele.m_simp["labelname"];
    QString unitLabel=ele.m_simp["unitname"];
    int defaultValue=ele.m_simp["default"].toInt();
    bool editable=ele.m_simp["editable"].toInt();
    QString tips=ele.m_simp["tips"];
    QString dataType=ele.m_simp["datatype"];
    int width=ele.m_simp["width"].toInt();
    int height=ele.m_simp["height"].toInt();

    QSpinBox * pSpin =new QSpinBox(parentW);
    pSpin->setFixedWidth(width);
    pSpin->setFixedHeight(height);
    pSpin->setEnabled(editable);
    pSpin->setObjectName(nameSpin);
    //设置默认值
    pSpin->setMaximum(ele.m_simpRange["maxvalue"].toInt());
    pSpin->setMinimum(ele.m_simpRange["minvalue"].toInt());
    pSpin->setSingleStep(ele.m_simpRange["singlestepvalue"].toInt());

    pSpin->setValue(defaultValue);
    //设置取值范围
    pSpin->setToolTip(tips);
    QLabel* pLabelname = new QLabel(parentW);
    pLabelname->setText(nameLabel);

    QLabel* pLabelunit = new QLabel(parentW);
    pLabelunit->setText(unitLabel);

    curlayout->addWidget(pLabelname,indexH,indexW+0);
    curlayout->addWidget(pSpin,indexH,indexW+1);
    curlayout->addWidget(pLabelunit,indexH,indexW+2);

    curlayout->setAlignment(pLabelname,Qt::AlignRight);
    curlayout->setAlignment(pSpin,Qt::AlignLeft);
    curlayout->setAlignment(pLabelunit,Qt::AlignLeft);
}

// 用于占位
void paraControl::CreateLabel(WidgetELEM &ele)
{
    QString nameSpin= ele.m_simp["name"];
    int indexH=ele.m_simp["indexH"].toInt();
    int indexW=ele.m_simp["indexW"].toInt();
    QString defaultValue=ele.m_simp["default"];
    bool editable=ele.m_simp["editable"].toInt();
    int width=ele.m_simp["width"].toInt();
    int height=ele.m_simp["height"].toInt();

    QLabel * pSpin =new QLabel(parentW);
    pSpin->setFixedWidth(width);
    pSpin->setFixedHeight(height);
    pSpin->setEnabled(editable);
    pSpin->setObjectName(nameSpin);

    pSpin->setText(defaultValue);
    //设置取值范围
    curlayout->addWidget(pSpin,indexH,indexW+1);
    curlayout->setAlignment(pSpin,Qt::AlignLeft);
}

void paraControl::ApplyParaValue(ParaMRI mypara)
{
	if (mypara.Seqtype == seqTypeEnum::SE)
	{
		SetSEPara(mypara);
	}
	else if(mypara.Seqtype==seqTypeEnum::IR)
	{
		SetIRPara(mypara);
	}
}

void paraControl::SetSEPara(ParaMRI mypara)
{
	QString value;
	value = QString::number(mypara.TE);
	SetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TEValue", value);

	value = QString::number(mypara.TR);
	SetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TRValue", value);

	value = mypara.description;
	SetSingleValue(paraTypeEnum::TYPE_LINEEDIT, "description", value);
}

void paraControl::SetIRPara(ParaMRI mypara)
{
	QString value;
	value = QString::number(mypara.TE);
	SetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TEValue", value);

	value = QString::number(mypara.TR);
	SetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TRValue", value);

	value = QString::number(mypara.TI);
	SetSingleValue(paraTypeEnum::TYPE_SPINBOX, "TIValue", value);

	value = mypara.description;
	SetSingleValue(paraTypeEnum::TYPE_LINEEDIT, "description", value);
}

void paraControl::SetGEPara(ParaMRI mypara)
{
}


