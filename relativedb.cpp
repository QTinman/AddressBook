#include "relativedb.h"
#include "ui_relativedb.h"
#include "mainwindow.h"
#include "delegate.h"
#include "expdelegate.h"
#include "tableprinter.h"
#include "printborder.h"
#include <QMessageBox>
#include <QPainter>
#include <QPrinter>
#include <QPrintPreviewDialog>



relativedb::relativedb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::relativedb)
{
    ui->setupUi(this);

    if (loadsettings("appnavn") == "") this->setWindowTitle(tr("Address book"));
    else this->setWindowTitle(loadsettings("appnavn").toString());
    timer = new QTimer(this);
    timer->setInterval(3000);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), SLOT(clearlabel()));
    timer->start();
    QString dbase=loadsettings("relativedb").toString();
    if (!db.tables().contains( QString(dbase) )) createdb();
    model = new QSqlTableModel(parent,db);
    initializeModel(model);
    ui->tableView->setModel(model);
    ui->label->clear();
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(model->fieldIndex("ID"), true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->show();
}

relativedb::~relativedb()
{
    timer->stop();
    delete ui;
}

QVariant relativedb::loadsettings(QString settings)
{
    QVariant returnvar;
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup(appgroup);
    returnvar = appsettings.value(settings);
    appsettings.endGroup();
    return returnvar;
}

void relativedb::savesettings(QString settings, QVariant attr)
{
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup(appgroup);
    appsettings.setValue(settings,attr);
    appsettings.endGroup();
}

void relativedb::initializeModel(QSqlTableModel *model)
{
    QString dbase=loadsettings("relativedb").toString().toLower();
    model->setTable(dbase);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString sQuery = "id='" + QString::number(id)+"'";
    int col = model->columnCount(),i2=0;
    model->setFilter(sQuery);
    QString felt[5] = {};
    if (model->headerData(0,Qt::Horizontal).toString().toLower() == "id") i2=1;
    for (int i=0;i<col;i++) {
        felt[i]=loadsettings("felt"+QString::number(i)+"_2").toString().toLower();
        felt[i] = felt[i].mid(0,1).toUpper() + felt[i].mid(1,felt[i].length()-1);
        if(felt[i] !="") model->setHeaderData(i2, Qt::Horizontal, QObject::tr(felt[i].toStdString().c_str())); //set headers from settings

        if (felt[i]=="Number") ui->tableView->setItemDelegateForColumn(i,new Delegate);
        else ui->tableView->setItemDelegateForColumn(i,new ItemDelegate);

        if (i2<=col) i2++; //i2 for skip Id column
    }

    model->select();
}


void relativedb::createdb()
{
    QString felt[5] = {};
    QString cfelt[5] = {};
    QString dbase=loadsettings("relativedb").toString().toLower();
    QString createTables = "create table "+dbase+"(id integer key)";
    for (int i=0;i<5;i++) {
        felt[i]=loadsettings("felt"+QString::number(i)+"_2").toString().toLower();
        felt[i].replace(" ","_");
        cfelt[i]=loadsettings("cfelt"+QString::number(i)+"_2").toString();
        if (cfelt[i] == "Number") cfelt[i] = "integer";
        else if (cfelt[i] == "String") cfelt[i] = "varchar(25)";
        else if (cfelt[i] == "Date") cfelt[i] = "varchar(12)";
        else if (cfelt[i] == "Time") cfelt[i] = "varchar(7)";
        else if (cfelt[i] == "LongString") cfelt[i] = "varchar(50)";
        if (felt[i] !="") createTables = createTables.mid(0,createTables.length()-1) + ", " + felt[i]+" "+cfelt[i]+")";
    }
    QSqlQuery qry(db);
    if (!qry.exec(createTables)) ui->label->setText(tr("Error ")+qry.lastError().text());
    qry.finish();
}


void relativedb::on_ny_clicked() //add record
{
    ui->label->setText(tr("New record"));
    timer->start();
    QModelIndex index;
    QDate cd = QDate::currentDate();
    QTime ct = QTime::currentTime();
    int row = model->rowCount(index);
    int col = model->columnCount();
    row = model->rowCount(QModelIndex());
    model->insertRow(model->rowCount(QModelIndex()));
    model->setData(model->index(row,0),id,Qt::EditRole);
    QString fieldstr="";
    for (int i=0;i<col;i++) {
        if (loadsettings("cfelt"+QString::number(i)+"_2").toString() == "Date")
            fieldstr = cd.toString(Qt::SystemLocaleShortDate);
        if (loadsettings("cfelt"+QString::number(i)+"_2").toString() == "Time")
            fieldstr = ct.toString(Qt::SystemLocaleShortDate);
        model->setData(model->index(row,i+1),fieldstr,Qt::EditRole);
        fieldstr="";
    }
}

void relativedb::on_slett_clicked()
{
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    //qDebug() << indexes.isEmpty();
    if (!indexes.isEmpty()) {
        QModelIndex index = indexes.at(0);
        model->removeRows(index.row(),1);
        ui->label->setText(tr("Mark deleted"));
    } else ui->label->setText(tr("Select record"));

    timer->start();
}

void relativedb::on_Lagre_clicked()
{

    ui->label->setText(tr("Submitted"));
    timer->start();
    if (!model->submitAll())  ui->label->setText(model->lastError().text());

}

void relativedb::on_Angre_clicked()
{
    model->revertAll();
    ui->label->setText(tr("Reverted"));
    //timer.singleShot(3000, [&](){ ui->label->clear();});
    timer->start();
}

void relativedb::cellSelected(int nRow, int nCol)
{
 QMessageBox::information(this, "",
 "Cell at row "+QString::number(nRow)+
 " column "+QString::number(nCol)+
 " was double clicked.");
}

void relativedb::clearlabel()
{
    ui->label->clear();
}

void relativedb::on_skrivut_clicked()
{
    QPrintPreviewDialog * printPreview = new QPrintPreviewDialog(this);
    printPreview->setGeometry(loadsettings("print_position").toRect());
    connect(printPreview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(doPrint(QPrinter *)));
    printPreview->exec();
    savesettings("print_position",printPreview->geometry());
}

void relativedb::doPrint(QPrinter * printer)
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
