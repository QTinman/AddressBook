#include "mainwindow.h"
#include "printborder.h"
#include <QInputDialog>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator t;
    PrintBorder *loads = new PrintBorder;
    QString lang = loads->loadsettings("language").toString();
    if (lang == "") {
        QStringList languages;
        languages << "English" << "Norsk";
        lang = QInputDialog::getItem(NULL,"Select Language","Language",languages);
        loads->savesettings("language",lang);
    }
    if (lang == "Norsk") t.load(":/translate/norsk.qm");
    if (lang != "English") a.installTranslator(&t);

    MainWindow w;
    w.show();
    return a.exec();
}
