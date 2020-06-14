#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QSettings>
#include <QDebug>
#include "convertdialog.h"
#include "printborder.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    load_data();
    load_relativedata();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    save_data();
    save_relativedata();

}

void SettingsDialog::load_data()
{
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup("database");
    QList<QString> combolist;
    combolist.append("String");
    combolist.append("Number");


    ui->cfelt0->addItems(combolist);
    ui->cfelt1->addItems(combolist);
    ui->cfelt2->addItems(combolist);
    ui->cfelt3->addItems(combolist);
    ui->cfelt4->addItems(combolist);
    ui->cfelt5->addItems(combolist);
    ui->databasenavn->setText(appsettings.value("databasenavn").toString());
    ui->dbTable->setText(appsettings.value("dbTable").toString());
    ui->appnavn->setText(appsettings.value("appnavn").toString());
    ui->felt0->setText(appsettings.value("felt0").toString());
    ui->felt1->setText(appsettings.value("felt1").toString());
    ui->felt2->setText(appsettings.value("felt2").toString());
    ui->felt3->setText(appsettings.value("felt3").toString());
    ui->felt4->setText(appsettings.value("felt4").toString());
    ui->felt5->setText(appsettings.value("felt5").toString());
    if (appsettings.value("cfelt0").toString() == "Number") ui->cfelt0->setCurrentIndex(1);
    if (appsettings.value("cfelt1").toString() == "Number") ui->cfelt1->setCurrentIndex(1);
    if (appsettings.value("cfelt2").toString() == "Number") ui->cfelt2->setCurrentIndex(1);
    if (appsettings.value("cfelt3").toString() == "Number") ui->cfelt3->setCurrentIndex(1);
    if (appsettings.value("cfelt4").toString() == "Number") ui->cfelt4->setCurrentIndex(1);
    if (appsettings.value("cfelt5").toString() == "Number") ui->cfelt5->setCurrentIndex(1);
    appsettings.endGroup();
}

void SettingsDialog::load_relativedata()
{
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup("database");
    QList<QString> combolist;
    combolist.append("String");
    combolist.append("Number");
    combolist.append("Date");
    combolist.append("Time");
    combolist.append("LongString");
    ui->cfelt0_2->addItems(combolist);
    ui->cfelt1_2->addItems(combolist);
    ui->cfelt2_2->addItems(combolist);
    ui->cfelt3_2->addItems(combolist);
    ui->cfelt4_2->addItems(combolist);
    ui->cfelt5_2->addItems(combolist);
    ui->relativedb->setText(appsettings.value("relativedb").toString());
    ui->felt0_2->setText(appsettings.value("felt0_2").toString());
    ui->felt1_2->setText(appsettings.value("felt1_2").toString());
    ui->felt2_2->setText(appsettings.value("felt2_2").toString());
    ui->felt3_2->setText(appsettings.value("felt3_2").toString());
    ui->felt4_2->setText(appsettings.value("felt4_2").toString());
    ui->felt5_2->setText(appsettings.value("felt5_2").toString());
    if (appsettings.value("cfelt0_2").toString() == "Number") ui->cfelt0_2->setCurrentIndex(1);
    if (appsettings.value("cfelt1_2").toString() == "Number") ui->cfelt1_2->setCurrentIndex(1);
    if (appsettings.value("cfelt2_2").toString() == "Number") ui->cfelt2_2->setCurrentIndex(1);
    if (appsettings.value("cfelt3_2").toString() == "Number") ui->cfelt3_2->setCurrentIndex(1);
    if (appsettings.value("cfelt4_2").toString() == "Number") ui->cfelt4_2->setCurrentIndex(1);
    if (appsettings.value("cfelt5_2").toString() == "Number") ui->cfelt5_2->setCurrentIndex(1);
    if (appsettings.value("cfelt0_2").toString() == "Date") ui->cfelt0_2->setCurrentIndex(2);
    if (appsettings.value("cfelt1_2").toString() == "Date") ui->cfelt1_2->setCurrentIndex(2);
    if (appsettings.value("cfelt2_2").toString() == "Date") ui->cfelt2_2->setCurrentIndex(2);
    if (appsettings.value("cfelt3_2").toString() == "Date") ui->cfelt3_2->setCurrentIndex(2);
    if (appsettings.value("cfelt4_2").toString() == "Date") ui->cfelt4_2->setCurrentIndex(2);
    if (appsettings.value("cfelt5_2").toString() == "Date") ui->cfelt5_2->setCurrentIndex(2);
    if (appsettings.value("cfelt0_2").toString() == "Time") ui->cfelt0_2->setCurrentIndex(3);
    if (appsettings.value("cfelt1_2").toString() == "Time") ui->cfelt1_2->setCurrentIndex(3);
    if (appsettings.value("cfelt2_2").toString() == "Time") ui->cfelt2_2->setCurrentIndex(3);
    if (appsettings.value("cfelt3_2").toString() == "Time") ui->cfelt3_2->setCurrentIndex(3);
    if (appsettings.value("cfelt4_2").toString() == "Time") ui->cfelt4_2->setCurrentIndex(3);
    if (appsettings.value("cfelt5_2").toString() == "Time") ui->cfelt5_2->setCurrentIndex(3);
    if (appsettings.value("cfelt0_2").toString() == "LongString") ui->cfelt0_2->setCurrentIndex(4);
    if (appsettings.value("cfelt1_2").toString() == "LongString") ui->cfelt1_2->setCurrentIndex(4);
    if (appsettings.value("cfelt2_2").toString() == "LongString") ui->cfelt2_2->setCurrentIndex(4);
    if (appsettings.value("cfelt3_2").toString() == "LongString") ui->cfelt3_2->setCurrentIndex(4);
    if (appsettings.value("cfelt4_2").toString() == "LongString") ui->cfelt4_2->setCurrentIndex(4);
    if (appsettings.value("cfelt5_2").toString() == "LongString") ui->cfelt5_2->setCurrentIndex(4);
    appsettings.endGroup();
}

void SettingsDialog::save_data()
{
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup("database");
    if (ui->databasenavn->text()!="") appsettings.setValue("databasenavn",QVariant(ui->databasenavn->text().toLower()));
    if (ui->appnavn->text()!="") appsettings.setValue("appnavn",QVariant(ui->appnavn->text()));
    if (ui->dbTable->text()!="") appsettings.setValue("dbTable",QVariant(ui->dbTable->text()));
    if (ui->felt0->text()!="") appsettings.setValue("felt0",QVariant(ui->felt0->text().toLower()));
    if (ui->felt1->text()!="") appsettings.setValue("felt1",QVariant(ui->felt1->text().toLower()));
    if (ui->felt2->text()!="") appsettings.setValue("felt2",QVariant(ui->felt2->text().toLower()));
    if (ui->felt3->text()!="") appsettings.setValue("felt3",QVariant(ui->felt3->text().toLower()));
    if (ui->felt4->text()!="") appsettings.setValue("felt4",QVariant(ui->felt4->text().toLower()));
    if (ui->felt5->text()!="") appsettings.setValue("felt5",QVariant(ui->felt5->text().toLower()));
    if (ui->felt0->text()=="") appsettings.remove("felt0");
    if (ui->felt1->text()=="") appsettings.remove("felt1");
    if (ui->felt2->text()=="") appsettings.remove("felt2");
    if (ui->felt3->text()=="") appsettings.remove("felt3");
    if (ui->felt4->text()=="") appsettings.remove("felt4");
    if (ui->felt5->text()=="") appsettings.remove("felt5");
    appsettings.setValue("cfelt0",QVariant(ui->cfelt0->currentText()));
    appsettings.setValue("cfelt1",QVariant(ui->cfelt1->currentText()));
    appsettings.setValue("cfelt2",QVariant(ui->cfelt2->currentText()));
    appsettings.setValue("cfelt3",QVariant(ui->cfelt3->currentText()));
    appsettings.setValue("cfelt4",QVariant(ui->cfelt4->currentText()));
    appsettings.setValue("cfelt5",QVariant(ui->cfelt5->currentText()));
    if (ui->felt0->text()=="") appsettings.remove("cfelt0");
    if (ui->felt1->text()=="") appsettings.remove("cfelt1");
    if (ui->felt2->text()=="") appsettings.remove("cfelt2");
    if (ui->felt3->text()=="") appsettings.remove("cfelt3");
    if (ui->felt4->text()=="") appsettings.remove("cfelt4");
    if (ui->felt5->text()=="") appsettings.remove("cfelt5");
    appsettings.endGroup();
}

void SettingsDialog::save_relativedata()
{
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup("database");
    if (ui->relativedb->text()!="") appsettings.setValue("relativedb",QVariant(ui->relativedb->text().toLower()));
    if (ui->felt0_2->text()!="") appsettings.setValue("felt0_2",QVariant(ui->felt0_2->text().toLower()));
    if (ui->felt1_2->text()!="") appsettings.setValue("felt1_2",QVariant(ui->felt1_2->text().toLower()));
    if (ui->felt2_2->text()!="") appsettings.setValue("felt2_2",QVariant(ui->felt2_2->text().toLower()));
    if (ui->felt3_2->text()!="") appsettings.setValue("felt3_2",QVariant(ui->felt3_2->text().toLower()));
    if (ui->felt4_2->text()!="") appsettings.setValue("felt4_2",QVariant(ui->felt4_2->text().toLower()));
    if (ui->felt5_2->text()!="") appsettings.setValue("felt5_2",QVariant(ui->felt5_2->text().toLower()));
    if (ui->felt0_2->text()=="") appsettings.remove("felt0_2");
    if (ui->felt1_2->text()=="") appsettings.remove("felt1_2");
    if (ui->felt2_2->text()=="") appsettings.remove("felt2_2");
    if (ui->felt3_2->text()=="") appsettings.remove("felt3_2");
    if (ui->felt4_2->text()=="") appsettings.remove("felt4_2");
    if (ui->felt5_2->text()=="") appsettings.remove("felt5_2");
    appsettings.setValue("cfelt0_2",QVariant(ui->cfelt0_2->currentText()));
    appsettings.setValue("cfelt1_2",QVariant(ui->cfelt1_2->currentText()));
    appsettings.setValue("cfelt2_2",QVariant(ui->cfelt2_2->currentText()));
    appsettings.setValue("cfelt3_2",QVariant(ui->cfelt3_2->currentText()));
    appsettings.setValue("cfelt4_2",QVariant(ui->cfelt4_2->currentText()));
    appsettings.setValue("cfelt5_2",QVariant(ui->cfelt5_2->currentText()));
    if (ui->felt0_2->text()=="") appsettings.remove("cfelt0_2");
    if (ui->felt1_2->text()=="") appsettings.remove("cfelt1_2");
    if (ui->felt2_2->text()=="") appsettings.remove("cfelt2_2");
    if (ui->felt3_2->text()=="") appsettings.remove("cfelt3_2");
    if (ui->felt4_2->text()=="") appsettings.remove("cfelt4_2");
    if (ui->felt5_2->text()=="") appsettings.remove("cfelt5_2");
    appsettings.endGroup();
}

void SettingsDialog::on_pushButton_clicked() //Convert
{
    convertDialog convertdialog;
    convertdialog.setModal(true);
    convertdialog.exec();
    PrintBorder * load = new PrintBorder;
    ui->databasenavn->setText(load->loadsettings("databasenavn").toString());
    ui->dbTable->setText(load->loadsettings("dbTable").toString());
}
