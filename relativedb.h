#ifndef RELATIVEDB_H
#define RELATIVEDB_H

#include <QDialog>
#include <QtSql>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include "delegate.h"


namespace Ui {
class relativedb;
}

class relativedb : public QDialog
{
    Q_OBJECT

public:
    explicit relativedb(QWidget *parent = nullptr);
    ~relativedb();

private slots:
    void on_ny_clicked();

    void on_slett_clicked();

    void on_Lagre_clicked();

    void on_Angre_clicked();
    void cellSelected(int nRow, int nCol);
    void clearlabel();

    void on_skrivut_clicked();
    void doPrint(QPrinter * printer);
private:
    Ui::relativedb *ui;
    Delegate *spdelegate;
    //QSqlDatabase dbr;
    QAbstractTableModel * amodel;
    QSqlTableModel * model;
    QTimer *timer;
    //void initializeModel(QSqlTableModel *model);
    QVariant loadsettings(QString settings);
    void savesettings(QString settings, QVariant attr);
    void initializeModel(QSqlTableModel *model);
    void createdb();


};

#endif // RELATIVEDB_H
