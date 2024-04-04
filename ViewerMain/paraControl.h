//参数界面xml到参数界面的转化
#ifndef PARACONTROL_H
#define PARACONTROL_H
#include "global.h"
#include "dataMode.h"

#include <QObject>
#include <QMainWindow>
#include <QByteArray>
#include <QLayout>
#include <QString>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QListView>

//控件类型
enum paraTypeEnum {
	TYPE_LINEEDIT,
	TYPE_COMBOBOX,
	TYPE_SPINBOX,
	TYPE_CHECKBOX,
	TYPE_TABWIDGET,
	TYPE_LABEL
};

//控件详细信息
typedef struct WidgetELEM
{
	QString strType;//控件类型,是什么控件
	QMap<QString, QString> m_simp;//控件元素
	QMap<QString, QString> m_simpRange;//取值范围
}WidgetELEM;

typedef struct WidEmList //控件信息vector
{
	QVector<QPair<QString, WidgetELEM>> m_mapList; // 控件名字 ， 控件信息
}WidEmList;


class paraControl : public QTabWidget
{
   Q_OBJECT
public:
    paraControl(QTabWidget *parent = 0);
    void CreateUIFromXml(QString  strXmlFile);
	void ApplyParaValue(ParaMRI mypara);
	void SetSEPara(ParaMRI mypara);
	void SetIRPara(ParaMRI mypara);
	void SetGEPara(ParaMRI mypara);
	void GetSEPara(ParaMRI &mypara);
	void GetIRPara(ParaMRI &mypara);
	void GetGEPara(ParaMRI &mypara);

private:
	void GetSingleValue(paraTypeEnum type, QString name, QString &value);
	void SetSingleValue(paraTypeEnum type, QString name, QString value);
    void AnalyzeParamsXml(QString strFile, WidEmList& myList);
    void InitTabWidget(WidgetELEM &ele);
    void CreateLineEdit(WidgetELEM &ele);
    void CreateSpinBox(WidgetELEM &ele);
    void CreateCombox(WidgetELEM &ele);
    void CreateCheckBox(WidgetELEM &ele);
    void CreateLabel(WidgetELEM &ele);

    WidEmList myList;//保存从xml文件读取的变量值
    QMap<QString,QString> m_para;//参数
    QList<QMap<QString,QString>> paraList;//没用
    QMap<QString,paraTypeEnum> m_mapControl;//字符串到type的映射
    QTabWidget *parentW;
    int curTabnum=0;
    QVector<QWidget *> mywidget;
    QVector<QGridLayout*> mygridlayout;
    QGridLayout *curlayout;
};

#endif // PARACONTROL_H
