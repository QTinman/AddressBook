#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include <QPainter>
#include "tableprinter.h"
#include "printborder.h"
#include <QInputDialog>

int id;
QString appgroup;
QSqlDatabase db;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE","vskv");
    appgroup = QDir::currentPath();
    int l = appgroup.lastIndexOf("/");
    appgroup = appgroup.mid(l+1,appgroup.length()-l)+"_database";
    QSettings appsettings("QTinman","adressbok");
    setGeometry(loadsettings("position").toRect());
    if (loadsettings("appnavn") == "") this->setWindowTitle(tr("Address book"));
    else this->setWindowTitle(loadsettings("appnavn").toString());
    if (loadsettings("databasenavn").toString() == "") on_instillinger_clicked();
    if (loadsettings("relativedb").toString() == "") ui->relativedb->hide();
    else ui->relativedb->setText(loadsettings("relativedb").toString().mid(0,1).toUpper()+loadsettings("relativedb").toString().mid(1,loadsettings("relativedb").toString().length()-1));
    QString vskv=loadsettings("databasenavn").toString();

    //db.setHostName("localhost");
    db.setDatabaseName(vskv);

    QString dbase=loadsettings("dbTable").toString();
    if (!db.open()) {
         qDebug() << db.lastError();
    } else if ( !db.tables().contains( QString(dbase) ))  createdb();
    model = new QSqlTableModel(this,db);
    connect(model,SIGNAL(beforeDelete(const int &)),this,SLOT(deleteRelative(const int&)));
    if (db.open()) {
        initializeModel(model);
        ui->tableView->setModel(model);
        ui->label->clear();
        ui->tableView->setSortingEnabled(true);
        ui->tableView->setColumnHidden(model->fieldIndex("ID"), true);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView->show();
    } else ui->label->setText(tr("Error ")+db.lastError().text());
}

MainWindow::~MainWindow()
{
    savesettings("position",this->geometry());
    db.close();
    delete ui;
}

void MainWindow::initializeModel(QSqlTableModel *model)
{
    model->setTable(loadsettings("dbTable").toString());
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    int col = model->columnCount(),i2=0;
    QString felt[5] = {};
    if (model->headerData(0,Qt::Horizontal).toString().toLower() == "id") i2=1;
    for (int i=0;i<=col;i++) {
        felt[i]=loadsettings("felt"+QString::number(i)).toString().toLower();
        felt[i] = felt[i].mid(0,1).toUpper() + felt[i].mid(1,felt[i].length()-1);
        if(felt[i] !="") model->setHeaderData(i2, Qt::Horizontal, QObject::tr(felt[i].toStdString().c_str()));
        i2++;
    }
    model->select();
}



void MainWindow::createdb()
{
    QString felt[5] = {};
    QString cfelt[5] = {};
    QString dbase=loadsettings("dbTable").toString().toLower();
    QString createTables = "create table "+dbase+"(id integer primary key)";
    if (db.open()) {
        for (int i=0;i<5;i++) {
            felt[i]=loadsettings("felt"+QString::number(i)).toString().toLower();
            felt[i].replace(" ","_");
            cfelt[i]=loadsettings("cfelt"+QString::number(i)).toString();
            if (cfelt[i] == "Number") cfelt[i] = "integer";
            else if (cfelt[i] == "String") cfelt[i] = "varchar(25)";
            else if (cfelt[i] == "Date") cfelt[i] = "varchar(12)";
            else if (cfelt[i] == "Time") cfelt[i] = "varchar(7)";
            else if (cfelt[i] == "LongString") cfelt[i] = "varchar(50)";
            if (felt[i] !="") createTables = createTables.mid(0,createTables.length()-1) + ", " + felt[i]+" "+cfelt[i]+")";
        }
    } else ui->label->setText(tr("Error ")+db.lastError().text());
    QSqlQuery qry(db);
    if (!qry.exec(createTables)) ui->label->setText("Error "+qry.lastError().text());
    qry.finish();
    db.close();
}

QVariant MainWindow::loadsettings(QString settings)
{
    QVariant returnvar;
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup(appgroup);
    returnvar = appsettings.value(settings);
    appsettings.endGroup();
    return returnvar;
}

void MainWindow::savesettings(QString settings, QVariant attr)
{
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup(appgroup);
    appsettings.setValue(settings,attr);
    appsettings.endGroup();
}

void MainWindow::on_instillinger_clicked()
{
    SettingsDialog instillinger;
    instillinger.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    instillinger.exec();
}

void MainWindow::on_ny_clicked()
{
    ui->label->setText(tr("New record"));
    QTimer::singleShot(3000, [&](){ ui->label->clear();});
    model->insertRow(model->rowCount(QModelIndex()));
}

void MainWindow::on_lagre_clicked()
{
    ui->label->setText(tr("Submitted"));
    QTimer::singleShot(3000, [&](){ ui->label->clear();});

    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    if (!indexes.isEmpty()) {
        QModelIndex index = indexes.at(0);
        id = model->data(index.sibling(index.row(),0)).toInt();
    }
    if (!model->submitAll())  ui->label->setText(model->lastError().text());
}

void MainWindow::on_angre_clicked()
{
    model->revertAll();
    ui->label->setText(tr("Reverted"));
    QTimer::singleShot(3000, [&](){ ui->label->clear();});
}

void MainWindow::on_slette_clicked()
{

    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    //qDebug() << indexes.isEmpty();
    if (!indexes.isEmpty()) {
        QModelIndex index = indexes.at(0);
        model->removeRows(index.row(),1);
        ui->label->setText(tr("Mark deleted"));
    } else ui->label->setText(tr("Select record"));
    QTimer::singleShot(3000, [&](){ ui->label->clear();});

}

void MainWindow::doPrint(QPrinter * printer)
{

    // ------------------ simplest example --------------------------

    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << tr("can't start printer");
        return;
    }
    // print table
    TablePrinter tablePrinter(&painter, printer);
    int col = model->columnCount();
    //QVector<int> columnStretch = QVector<int>() << 2 << 5 << 10;
    QVector<QString> headers;
    QVector<int> columnStretch;
    QFont header; // font for headers
    header.setBold(true);
    QFont content; // font for content
    content.setItalic(true);
    tablePrinter.setHeadersFont(header);
    tablePrinter.setContentFont(content);
    PrintBorder *printB = new PrintBorder;
    printB->pageNumber = 1;
    tablePrinter.setPagePrepare(printB);
    //uglyPainter.translate(0, 60); // start print point
    for (int i=0;i<col;i++) {
        headers << model->headerData(i,Qt::Horizontal).toString();
        columnStretch << ui->tableView->columnWidth(i);
    }
    painter.translate(0, 40); // start print point
    tablePrinter.setCellMargin(10, 5, 5, 5);
    tablePrinter.setPageMargin(40, 40, 40, 40);
    if(!tablePrinter.printTable(ui->tableView->model(), columnStretch, headers)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
    delete printB;
}

void MainWindow::on_pushButton_clicked() //print
{

    QPrintPreviewDialog * printPreview = new QPrintPreviewDialog(this);
    printPreview->setGeometry(loadsettings("print_position").toRect());
    connect(printPreview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(doPrint(QPrinter *)));
    printPreview->exec();
    savesettings("print_position",printPreview->geometry());
}

void MainWindow::on_relativedb_clicked()
{
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    if (!indexes.isEmpty()) {
        QModelIndex index = indexes.at(0);
        id = model->data(index.sibling(index.row(),0)).toInt();
        relativedb relavdb;
        relavdb.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
        relavdb.exec();
    } else ui->label->setText(tr("Select record"));
    QTimer::singleShot(3000, [&](){ ui->label->clear();});
}

void MainWindow::on_action_Language_triggered()
{
    QStringList languages;
    languages << "English" << "Norsk";
    QString lang = QInputDialog::getItem(NULL,tr("Select Language"),tr("Language"),languages);
    savesettings("language",lang);
}

void MainWindow::deleteRelative(const int& row)
{
    QString relativdb = loadsettings("relativedb").toString();
    if (relativdb != "") {
        ui->tableView->selectRow(row);
        QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
        QModelIndex index = indexes.at(0);
        int rid = model->data(index.sibling(index.row(),0)).toInt();

        QString qryString = "DELETE FROM "+relativdb+" WHERE ID="+QString::number(rid)+";";
        QSqlQuery qry(db);
        if (!qry.exec(qryString)) ui->label->setText("Error "+qry.lastError().text());
        qry.finish();
    }
}
