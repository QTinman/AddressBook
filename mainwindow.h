#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include "relativedb.h"

extern int id;
extern QString appgroup;
extern QSqlDatabase db;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createdb();
    QVariant loadsettings(QString settings);
    void savesettings(QString settings, QVariant attr);

private slots:
    void on_instillinger_clicked();

    void on_ny_clicked();

    void on_lagre_clicked();

    void on_angre_clicked();

    void on_slette_clicked();

    void on_pushButton_clicked();

    void doPrint(QPrinter * printer);
    void on_relativedb_clicked();

    void on_action_Language_triggered();
    void deleteRelative(const int&);

private:
    Ui::MainWindow *ui;

    relativedb *relavdb;

    //QSqlDatabase db;
    QSqlTableModel * model;
    void initializeModel(QSqlTableModel *model);

};
#endif // MAINWINDOW_H
