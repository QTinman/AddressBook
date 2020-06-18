#include "printborder.h"
#include <QSettings>
#include "mainwindow.h"

// ----------------- example of inharitance from PagePrepare ---------------------


int PrintBorder::pageNumber = 0;

void PrintBorder::preparePage(QPainter *painter) { // print a border on each page
    QRect rec = painter->viewport();
    painter->setPen(QPen(QColor(0, 0, 0), 1));
    painter->drawRect(rec);
    painter->translate(10, painter->viewport().height() - 10);
    painter->drawText(0, 0, QString(MainWindow::tr("Page")+" %1").arg(pageNumber));
    pageNumber += 1;

}

// --------------------------------------------------------------------------------
QVariant PrintBorder::loadsettings(QString settings)
{
    appgroup = QDir::currentPath();
    int l = appgroup.lastIndexOf("/");
    appgroup = appgroup.mid(l+1,appgroup.length()-l)+"_database";
    QVariant returnvar;
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup(appgroup);
    returnvar = appsettings.value(settings);
    appsettings.endGroup();
    return returnvar;
}

void PrintBorder::savesettings(QString settings, QVariant attr)
{

    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup(appgroup);
    appsettings.setValue(settings,attr);
    appsettings.endGroup();
}
