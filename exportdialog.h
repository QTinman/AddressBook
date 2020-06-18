#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QtSql>
#include <QStandardItemModel>

namespace Ui {
class exportDialog;
}

class exportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit exportDialog(QWidget *parent = nullptr);
    ~exportDialog();

private slots:
    void on_export_2_clicked();
    void tablefilename();
private:
    Ui::exportDialog *ui;

    QSqlTableModel * sqlmodel;
    QStandardItemModel *csvModel;
};

#endif // EXPORTDIALOG_H
