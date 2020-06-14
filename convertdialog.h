#ifndef CONVERTDIALOG_H
#define CONVERTDIALOG_H

#include <QDialog>
#include <QtSql>
#include <QStandardItemModel>
namespace Ui {
class convertDialog;
}

class convertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit convertDialog(QWidget *parent = nullptr);
    ~convertDialog();

private slots:
    void on_selectdb_clicked();

    void on_pushButton_clicked();

    void on_convert_clicked();
    void lineeditslot();

    void on_addfield_clicked();

    void on_removefield_clicked();

private:
    Ui::convertDialog *ui;
    void hideall();
    void loadoutfields();
    void loadinfields();
    void createdb();
    void updateSettings();
    QSqlDatabase db;
    QSqlDatabase db_ut;
    QSqlTableModel * sqlmodel;
    QSqlTableModel * sql_utmodel;
    QStandardItemModel *csvModel;
};

#endif // CONVERTDIALOG_H
