#include "mainWindow.h"
#include <vtkAutoInit.h>
#include <QApplication>
#include <QTranslator>
#include <QFont>
#include <QFile>
#include <QtDebug>
#include "global.h"
#include "vtkObject.h"


VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);


int main(int argc, char *argv[])
{
	// APP 在最上面
	QApplication app(argc, argv);
	vtkObject::GlobalWarningDisplayOff();
	app.setQuitOnLastWindowClosed(true);
	app.setOrganizationName("APPname");
	app.setOrganizationDomain("none.com");
	app.setApplicationName("APPname");

	QTranslator t;
	t.load("dicomviewer_cn.qm");
	app.installTranslator(&t);

	//加载主题文件
	QFile themeFIle(apptheme);
	if (themeFIle.exists())
	{
		qDebug() << "theme File read OK";
		themeFIle.open(QFile::ReadOnly);
		QString styleSheet = QString::fromUtf8(themeFIle.readAll());
		app.setStyleSheet(styleSheet);
		themeFIle.close();
	}
	else
	{
		qDebug() << "theme error";
	}
	MainWindow mainwindow;
	mainwindow.showMaximized();
	return app.exec();
}
