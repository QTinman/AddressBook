#include "exportdialog.h"
#include "ui_exportdialog.h"
#include "mainwindow.h"
#include "printborder.h"

exportDialog::exportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exportDialog)
{
    ui->setupUi(this);


    ui->tables->addItems(db.tables());
    sqlmodel = new QSqlTableModel(this,db);
    sqlmodel->setTable(ui->tables->currentText());

    for (int i=0;i<sqlmodel->columnCount();i++)
    ui->sortorder->addItem(QString::number(i));
    ui->csv_file->setText(ui->tables->currentText()+".csv");
    //connect(ui->tables,SIGNAL(currentTextChanged(const int &)),this,SLOT(tablefilename(const int&)));
    connect(ui->tables, QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index){ tablefilename(); });
}

exportDialog::~exportDialog()
{
    delete ui;
}

void exportDialog::on_export_2_clicked()
{
    if (ui->csv_file->text() != "") {
        if (db.open()) {
            //PrintBorder * load = new PrintBorder;

            sqlmodel->setTable(ui->tables->currentText());
            sqlmodel->select();
            sqlmodel->sort(ui->sortorder->currentText().toInt(),Qt::AscendingOrder);
            QFile file(ui->csv_file->text());
            if (file.exists()) {
                ui->message->setText("Existing file deleted");
                file.remove();
            }
            QString csvheaders="";
            int col=sqlmodel->columnCount();
            for (int i=0;i<col;i++){
                if (i!=0 || ui->with_id->isChecked()) csvheaders+=sqlmodel->headerData(i,Qt::Horizontal).toString();
                if ((i!=col-1 && i!=0) || (i!=col-1 && ui->with_id->isChecked())) csvheaders+=",";
                //qDebug() << i << ui->with_id->isChecked();
            }
            if ( file.open(QIODevice::ReadWrite) ) {
                QTextStream stream( &file );
                stream << csvheaders << "\n";
                QString csvdata="";
                int row = sqlmodel->rowCount();
                int col = sqlmodel->columnCount();
                //qDebug() << row << " " << col;
                for (int i = 0; i < row ; ++i) {

                   for (int j = 0; j < col; j++) {
                       QVariant content = sqlmodel->data(sqlmodel->index(i, j,QModelIndex()), Qt::DisplayRole);
                       //qDebug() << content.toString();
                       if (j!=0 || ui->with_id->isChecked()) csvdata += content.toString();
                       if ((j!=col-1 && j!=0) || (j!=col-1 && ui->with_id->isChecked())) csvdata += ",";
                       //qDebug() << i << ui->with_id->isChecked();

                   }
                   stream << csvdata << "\n";
                   //qDebug() << csvdata;
                   csvdata="";
               }
            } else ui->message->setText(tr("Error ")+file.error());
            file.close();
        } else ui->message->setText(tr("Error ")+db.lastError().text());
    }  else ui->message->setText("Enter file name!");
}


void exportDialog::tablefilename()
{
    sqlmodel->setTable(ui->tables->currentText());
    ui->sortorder->clear();
    for (int i=0;i<sqlmodel->columnCount();i++)
    ui->sortorder->addItem(QString::number(i));
    ui->csv_file->setText(ui->tables->currentText()+".csv");
}
