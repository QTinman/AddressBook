#include "convertdialog.h"
#include "ui_convertdialog.h"
#include "printborder.h"
#include <QFileDialog>

convertDialog::convertDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::convertDialog)
{
    ui->setupUi(this);

    connect(ui->db_out,SIGNAL(editingFinished()),this,SLOT(lineeditslot()));
    connect(ui->db_in,SIGNAL(editingFinished()),this,SLOT(lineeditslot()));
    connect(ui->addfieldname,SIGNAL(editingFinished()),this,SLOT(lineeditslot()));
    hideall();
    loadinfields();

    //

}

convertDialog::~convertDialog()
{
    delete ui;
}

void convertDialog::hideall()
{
    ui->infield1->setDisabled(true);
    ui->infield2->setDisabled(true);
    ui->infield3->setDisabled(true);
    ui->infield4->setDisabled(true);
    ui->infield5->setDisabled(true);
    ui->infield6->setDisabled(true);

    ui->utfield1->setDisabled(true);
    ui->utfield2->setDisabled(true);
    ui->utfield3->setDisabled(true);
    ui->utfield4->setDisabled(true);
    ui->utfield5->setDisabled(true);
    ui->utfield6->setDisabled(true);
    ui->convert->setDisabled(true);
    ui->pushButton->setDisabled(true);
    ui->addfield->setDisabled(true);
    ui->removefield->setDisabled(true);
}

void convertDialog::loadinfields()
{
    //qDebug() << ui->
}

void convertDialog::loadoutfields()
{
    PrintBorder * load = new PrintBorder;
    QStringList fields;
    int i=1;
    QString field = load->loadsettings("felt0").toString();

    while (field != "") {
        fields << field;
        field = load->loadsettings("felt"+QString::number(i)).toString();
        i++;
    }
    ui->utfield1->clear();
    ui->utfield2->clear();
    ui->utfield3->clear();
    ui->utfield4->clear();
    ui->utfield5->clear();
    ui->utfield6->clear();
    ui->infield1->clear();
    ui->infield2->clear();
    ui->infield3->clear();
    ui->infield4->clear();
    ui->infield5->clear();
    ui->infield6->clear();
    ui->utfield1->addItems(fields);
    ui->utfield2->addItems(fields);
    ui->utfield3->addItems(fields);
    ui->utfield4->addItems(fields);
    ui->utfield5->addItems(fields);
    ui->utfield6->addItems(fields);
    if (ui->addfieldname->text() != "") ui->addfield->setEnabled(true);
    ui->removefield->setEnabled(true);
    if (i > 1) {
        ui->utfield1->setEnabled(true);
        ui->utfield1->setCurrentIndex(0);
        ui->infield1->setEnabled(true);
    }
    if (i > 2) {
        ui->utfield2->setEnabled(true);
        ui->utfield2->setCurrentIndex(1);
        ui->infield2->setEnabled(true);
    }
    if (i > 3) {
        ui->utfield3->setEnabled(true);
        ui->utfield3->setCurrentIndex(2);
        ui->infield3->setEnabled(true);
    }
    if (i > 4) {
        ui->utfield4->setEnabled(true);
        ui->utfield4->setCurrentIndex(3);
        ui->infield4->setEnabled(true);
    }
    if (i > 5) {
        ui->utfield5->setEnabled(true);
        ui->utfield5->setCurrentIndex(4);
        ui->infield5->setEnabled(true);
    }
    if (i > 6) {
        ui->utfield6->setEnabled(true);
        ui->utfield6->setCurrentIndex(5);
        ui->infield6->setEnabled(true);
    }

}

void convertDialog::on_selectdb_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open DB"), "./", tr("DB Files (*.db *.csv)"));
    ui->db_in->setText(fileName);
    if (fileName.mid(fileName.length()-3,3).toLower() == "csv") {
        ui->message->setText(tr("Csv selected"));
        ui->label_3->setText(tr("Separator"));
        ui->tables->addItems(QStringList() << "<comma>" << "<semicolon>" << "<tabulator>");
        int l = fileName.lastIndexOf("/");
        if (l != -1) {
            ui->db_out->setText(fileName.mid(l+1,fileName.length()-4-l)+"db");
            ui->db_out_table->setText(fileName.mid(l+1,fileName.length()-5-l));
        } else {
            ui->db_out->setText(fileName.mid(0,fileName.length()-3)+"db");
            ui->db_out_table->setText(fileName.mid(0,fileName.length()-4));
        }
    }else{
        db = QSqlDatabase::addDatabase("QSQLITE","convert_inn");
        db.setDatabaseName(fileName);
        ui->message->setText(tr("SqLite selected"));
        if (db.open()) ui->tables->addItems(db.tables());
        else ui->message->setText(tr("Error ")+db.lastError().text());
        int l = fileName.lastIndexOf("/");
        if (l != -1) {
            ui->db_out->setText(fileName.mid(l+1,fileName.length()-4-l)+"_1.db");
            ui->db_out_table->setText(fileName.mid(l+1,fileName.length()-4-l));
        } else {
            ui->db_out->setText(fileName.mid(0,fileName.length()-3)+"_1.db");
            ui->db_out_table->setText(fileName.mid(0,fileName.length()-3));
        }
        db.close();
    }
    QFile dbfile(ui->db_out->text());
    if (dbfile.exists()) ui->message->setText(tr("File exists! Convert will overwrite!"));
    else ui->message->clear();
    if (ui->db_in->text() != "") ui->pushButton->setEnabled(true);
    else ui->pushButton->setDisabled(true);
}

void convertDialog::on_pushButton_clicked() //load fields
{
    //QString database = ui->db_in->text();

    loadoutfields();
    QStringList fields;
    //QString field;
    //    ui->utfield1->count();
    if (ui->db_in->text().mid(ui->db_in->text().length()-3,3).toLower() == "csv") {
        csvModel = new QStandardItemModel(this);
        //csvModel->setColumnCount(3);  // Check!!!!!!!!!!!!!!!!!!!1
        QFile file(ui->db_in->text());

        if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
            qDebug() << "File not exists";
        } else {
            // Create a thread to retrieve data from a file
            QTextStream in(&file);
            QString line = in.readLine(),sepparator;
            if (ui->tables->currentText() == "<comma>") sepparator = ",";
            if (ui->tables->currentText() == "<semicomma>") sepparator = ";";
            if (ui->tables->currentText() == "<tabulator>") sepparator = "\t";
            //QStringList fields;
            // Adding to the model in line with the elements
            QList<QStandardItem *> standardItemsList;
            // consider that the line separated by semicolons into columns
            for (QString item : line.split(sepparator)) {
                fields << item;
            }
            csvModel->setColumnCount(fields.count());
            csvModel->setHorizontalHeaderLabels(fields);
            fields << "<none>";
            ui->infield1->addItems(fields);
            ui->infield2->addItems(fields);
            ui->infield3->addItems(fields);
            ui->infield4->addItems(fields);
            ui->infield5->addItems(fields);
            ui->infield6->addItems(fields);

            ui->infield1->setCurrentIndex(0);
            ui->infield2->setCurrentIndex(1);
            ui->infield3->setCurrentIndex(2);
            ui->infield4->setCurrentIndex(3);
            ui->infield5->setCurrentIndex(4);
            ui->infield6->setCurrentIndex(5);
            //Reads the data up to the end of file
            file.close();
        }
    } else {
        if (db.open()) {

            sqlmodel = new QSqlTableModel(this,db);
            sqlmodel->setTable(ui->tables->currentText());
            //qDebug() <<  sqlmodel->lastError();
            //qDebug() << ui->tables->currentText();
            int col=sqlmodel->columnCount();
            for (int i=0;i<col;i++){
                if (sqlmodel->headerData(i,Qt::Horizontal).toString().toLower() != "id") fields << sqlmodel->headerData(i,Qt::Horizontal).toString();
            }
            fields << "<none>";
            ui->infield1->addItems(fields);
            ui->infield2->addItems(fields);
            ui->infield3->addItems(fields);
            ui->infield4->addItems(fields);
            ui->infield5->addItems(fields);
            ui->infield6->addItems(fields);

            ui->infield1->setCurrentIndex(0);
            ui->infield2->setCurrentIndex(1);
            ui->infield3->setCurrentIndex(2);
            ui->infield4->setCurrentIndex(3);
            ui->infield5->setCurrentIndex(4);
            ui->infield6->setCurrentIndex(5);
        } else ui->message->setText(tr("Error ")+db.lastError().text());
    }
    if (ui->db_out->text() != "") ui->convert->setEnabled(true);
    else {
        ui->message->setText(tr("Output DB missing!"));
        ui->db_out->focusWidget();
    }

}

void convertDialog::on_convert_clicked()
{

    createdb();
    //db_ut = QSqlDatabase::addDatabase("QSQLITE","convert_ut");
    db_ut.setDatabaseName(ui->db_out->text());
    QStringList fields;
    db_ut.open();
    sql_utmodel = new QSqlTableModel(this,db_ut);
    sql_utmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString dbase=ui->db_out_table->text();
    sql_utmodel->setTable(dbase);

    if (ui->db_out->text() != "") {
        if (ui->db_in->text().mid(ui->db_in->text().length()-3,3).toLower() == "csv") {
            csvModel = new QStandardItemModel(this);
            //csvModel->setColumnCount(3);
            QFile file(ui->db_in->text());
            if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
                qDebug() << "File not exists";
            } else {
                // Create a thread to retrieve data from a file
                QTextStream in(&file);
                 QString line = in.readLine(),sepparator;
                 if (ui->tables->currentText() == "<comma>") sepparator = ",";
                 if (ui->tables->currentText() == "<semicomma>") sepparator = ";";
                 if (ui->tables->currentText() == "<tabulator>") sepparator = "\t";
                // Adding to the model in line with the elements
                QList<QStandardItem *> standardItemsList;
                // consider that the line separated by semicolons into columns
                for (QString item : line.split(sepparator)) {
                    fields << item;
                }
                csvModel->setHorizontalHeaderLabels(fields);

                //Reads the data up to the end of file
                while (!in.atEnd())
                {
                    QString line = in.readLine();
                    // Adding to the model in line with the elements
                    QList<QStandardItem *> standardItemsList;
                    // consider that the line separated by semicolons into columns
                    for (QString item : line.split(sepparator)) {
                        standardItemsList.append(new QStandardItem(item));
                    }
                    csvModel->insertRow(csvModel->rowCount(), standardItemsList);
                }
                file.close();
                QString infield[6] = {};
                infield[0]=ui->infield1->currentText();
                infield[1]=ui->infield2->currentText();
                infield[2]=ui->infield3->currentText();
                infield[3]=ui->infield4->currentText();
                infield[4]=ui->infield5->currentText();
                infield[5]=ui->infield6->currentText();
                int row = csvModel->rowCount();
                int col = csvModel->columnCount();
                //qDebug() << row << " " << col;
                for (int i = 0; i < row ; ++i) {
                    sql_utmodel->insertRow(sql_utmodel->rowCount(QModelIndex()));
                    sql_utmodel->setData(sql_utmodel->index(i, 0),QVariant(i),Qt::EditRole);
                   for (int j = 0; j < col; j++) {
                       QVariant content = csvModel->data(csvModel->index(i, j,QModelIndex()), Qt::DisplayRole);
                       for (int f=0;f<col;f++) {
                           if (infield[f] == csvModel->headerData(j,Qt::Horizontal).toString()) {
                               sql_utmodel->setData(sql_utmodel->index(i, f+1),content,Qt::EditRole);
                           }
                       }
                   }
               }
            }
        } else {
            sqlmodel = new QSqlTableModel(this,db);
            db.setDatabaseName(ui->db_in->text());

            if (db.open()) {
                sqlmodel->setTable(ui->tables->currentText());
                //sqlmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
                sqlmodel->select();
                //qDebug() <<  sqlmodel->lastError();
                //qDebug() << ui->tables->currentText();
                QString infield[6] = {};
                infield[0]=ui->infield1->currentText();
                infield[1]=ui->infield2->currentText();
                infield[2]=ui->infield3->currentText();
                infield[3]=ui->infield4->currentText();
                infield[4]=ui->infield5->currentText();
                infield[5]=ui->infield6->currentText();
                int row = sqlmodel->rowCount();
                int col = sqlmodel->columnCount();
                qDebug() << row << " " << col;
                for (int i = 0; i < row ; ++i) {
                    sql_utmodel->insertRow(sql_utmodel->rowCount(QModelIndex()));
                    sql_utmodel->setData(sql_utmodel->index(i, 0),QVariant(i),Qt::EditRole);
                   for (int j = 0; j < col; j++) {
                       QVariant content = sqlmodel->data(sqlmodel->index(i, j,QModelIndex()), Qt::DisplayRole);
                       for (int f=0;f<col;f++) {
                           if (infield[f] == sqlmodel->headerData(j,Qt::Horizontal).toString()) {
                               sql_utmodel->setData(sql_utmodel->index(i, f+1),content,Qt::EditRole);
                           }
                       }
                   }
               }
            } else ui->message->setText(tr("Error ")+db.lastError().text());
        }
        if (!sql_utmodel->submitAll()) ui->message->setText(tr("Error ")+sql_utmodel->lastError().text());
        db.close();
        //db_ut.removeDatabase("convert_inn");
    } else {
        ui->message->setText(tr("Output DB missing!"));
        ui->db_out->focusWidget();
    }
    db_ut.close();
    //db_ut.removeDatabase("convert_ut");
    if (ui->updateSettings->isChecked()) updateSettings();
}

void convertDialog::createdb()
{
    db_ut = QSqlDatabase::addDatabase("QSQLITE","convert_ut");
    QFile dbfile(ui->db_out->text());
    if (dbfile.exists()) dbfile.remove();
    db_ut.setDatabaseName(ui->db_out->text());
    QString felt[6] = {};
    QString cfelt[6] = {};
    PrintBorder * load = new PrintBorder;
    //QString dbase=load->loadsettings("databasenavn").toString().toLower();
    QString dbase=ui->db_out_table->text();
    QString utfield[6] = {};
    QString utcfield[6] = {};
    if (ui->utfield1->isEnabled()) utfield[0]=ui->utfield1->currentText();
    if (ui->utfield2->isEnabled()) utfield[1]=ui->utfield2->currentText();
    if (ui->utfield3->isEnabled()) utfield[2]=ui->utfield3->currentText();
    if (ui->utfield4->isEnabled()) utfield[3]=ui->utfield4->currentText();
    if (ui->utfield5->isEnabled()) utfield[4]=ui->utfield5->currentText();
    if (ui->utfield6->isEnabled()) utfield[5]=ui->utfield6->currentText();
    for (int i=0;i<6;i++) {
        felt[i]=load->loadsettings("felt"+QString::number(i)).toString().toLower();
        felt[i].replace(" ","_");
        cfelt[i]=load->loadsettings("cfelt"+QString::number(i)).toString();
    }
    QString createTables = "create table "+dbase+"(id integer primary key)";
    if (db_ut.open()) {

        for (int i=0;i<6;i++) {

            if (utfield[i]==felt[0]) utcfield[i] = cfelt[0];
            if (utfield[i]==felt[1]) utcfield[i] = cfelt[1];
            if (utfield[i]==felt[2]) utcfield[i] = cfelt[2];
            if (utfield[i]==felt[3]) utcfield[i] = cfelt[3];
            if (utfield[i]==felt[4]) utcfield[i] = cfelt[4];
            if (utfield[i]==felt[5]) utcfield[i] = cfelt[5];
            if (utcfield[i] == "Number") utcfield[i] = "integer";
            else if (utcfield[i] == "String") utcfield[i] = "varchar(25)";
            else if (utcfield[i] == "Date") utcfield[i] = "varchar(12)";
            else if (utcfield[i] == "Time") utcfield[i] = "varchar(7)";
            else if (utcfield[i] == "LongString") utcfield[i] = "varchar(50)";

        if (utfield[i] !="" && utfield[i] !="<none>") createTables = createTables.mid(0,createTables.length()-1) + ", " + utfield[i]+" "+utcfield[i]+")";

        }
    } else ui->message->setText(tr("Error ")+db_ut.lastError().text());
    //qDebug() << createTables;
    QSqlQuery qry(db_ut);
    if (!qry.exec(createTables)) ui->message->setText(tr("Error ")+qry.lastError().text());
    qry.finish();
    db_ut.close();
    //db_ut.removeDatabase("convert_ut");
}


void convertDialog::lineeditslot()
{
    QFile dbfile(ui->db_out->text());

    if (dbfile.exists()) ui->message->setText(tr("File exists! Convert will overwrite!"));
    else ui->message->clear();
    if (ui->addfieldname->text() != "") ui->addfield->setEnabled(true);
    else ui->addfield->setDisabled(true);
    if (ui->db_in->text() != "") ui->pushButton->setEnabled(true);
    else ui->pushButton->setDisabled(true);
    if (ui->db_out->text() != "" && ui->db_in->text() != "") ui->convert->setEnabled(true);
    else ui->convert->setDisabled(true);
}

void convertDialog::on_addfield_clicked()
{
    QStringList fields;
    int i;
    for (i=0;i<ui->utfield1->count();i++) {
        ui->utfield1->setCurrentIndex(i);
        if (ui->utfield1->currentText() != "<none>") fields << ui->utfield1->currentText();
    }
    fields << ui->addfieldname->text();
    //fields << "<none>";
    ui->utfield1->clear();
    ui->utfield2->clear();
    ui->utfield3->clear();
    ui->utfield4->clear();
    ui->utfield5->clear();
    ui->utfield6->clear();

    if (i==2) ui->utfield2->setEnabled(true);
    if (i==3) ui->utfield3->setEnabled(true);
    if (i==4) ui->utfield4->setEnabled(true);
    if (i==5) ui->utfield5->setEnabled(true);
    if (i==6) ui->utfield6->setEnabled(true);
    if (i==2) ui->infield2->setEnabled(true);
    if (i==3) ui->infield3->setEnabled(true);
    if (i==4) ui->infield4->setEnabled(true);
    if (i==5) ui->infield5->setEnabled(true);
    if (i==6) ui->infield6->setEnabled(true);

    ui->utfield1->addItems(fields);
    ui->utfield2->addItems(fields);
    ui->utfield3->addItems(fields);
    ui->utfield4->addItems(fields);
    ui->utfield5->addItems(fields);
    ui->utfield6->addItems(fields);
    ui->utfield1->setCurrentIndex(0);
    ui->utfield2->setCurrentIndex(1);
    ui->utfield3->setCurrentIndex(2);
    ui->utfield4->setCurrentIndex(3);
    ui->utfield5->setCurrentIndex(4);
    ui->utfield6->setCurrentIndex(5);

}

void convertDialog::on_removefield_clicked()
{
    QStringList fields;
    int i;
    for (i=0;i<ui->utfield1->count();i++) {
        ui->utfield1->setCurrentIndex(i);
        if (ui->utfield1->currentText() != "<none>") fields << ui->utfield1->currentText();
    }

    //fields << "<none>";
    ui->utfield1->clear();
    ui->utfield2->clear();
    ui->utfield3->clear();
    ui->utfield4->clear();
    ui->utfield5->clear();
    ui->utfield6->clear();

    if (i==2) ui->utfield2->setDisabled(true);
    if (i==3) ui->utfield3->setDisabled(true);
    if (i==4) ui->utfield4->setDisabled(true);
    if (i==5) ui->utfield5->setDisabled(true);
    if (i==6) ui->utfield6->setDisabled(true);
    if (i==2) ui->infield2->setDisabled(true);
    if (i==3) ui->infield3->setDisabled(true);
    if (i==4) ui->infield4->setDisabled(true);
    if (i==5) ui->infield5->setDisabled(true);
    if (i==6) ui->infield6->setDisabled(true);

    ui->utfield1->addItems(fields);
    ui->utfield2->addItems(fields);
    ui->utfield3->addItems(fields);
    ui->utfield4->addItems(fields);
    ui->utfield5->addItems(fields);
    ui->utfield6->addItems(fields);
    ui->utfield1->setCurrentIndex(0);
    ui->utfield2->setCurrentIndex(1);
    ui->utfield3->setCurrentIndex(2);
    ui->utfield4->setCurrentIndex(3);
    ui->utfield5->setCurrentIndex(4);
    ui->utfield6->setCurrentIndex(5);
}

void convertDialog::updateSettings()
{
    PrintBorder * save = new PrintBorder;
    save->savesettings("databasenavn",ui->db_out->text());
    save->savesettings("dbTable",ui->db_out_table->text());
}
