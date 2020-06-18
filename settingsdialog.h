#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_selectDB_clicked();

private:
    Ui::SettingsDialog *ui;
    void load_data();
    void load_relativedata();
    void save_data();
    void save_relativedata();
    QSqlTableModel * sqlmodel;
};

#endif // SETTINGSDIALOG_H
