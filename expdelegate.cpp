#include "expdelegate.h"

#include <QLineEdit>
#include <QIntValidator>
#include <QRegExpValidator>
#include "relativedb.h"
#include "mainwindow.h"

ItemDelegate::ItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *ItemDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    int col = index.column()-1;
    QString cfelt=loadsettings("cfelt"+QString::number(col)+"_2").toString();
    QRegExp ex;
    if (cfelt=="Date") ex.setPattern("(0?[1-9]|[12][0-9]|3[01])[./ -]?(0?[1-9]|1[0-2])[./ -]?(?:19|20)[0-9]{2}");
    if (cfelt=="Time") ex.setPattern("^(?:[01][0-9]|2[0-3])[-:h][0-5][0-9]$");

    //qDebug() << col << " " << cfelt << " " << ex.pattern();
    QLineEdit *editor = new QLineEdit(parent);
    if (cfelt!="String") editor->setValidator(new QRegExpValidator(ex));
    return editor;
}


void ItemDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    QString value =index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *line = static_cast<QLineEdit*>(editor);
        line->setText(value);
}


void ItemDelegate::setModelData(QWidget *editor,
                                QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    QLineEdit *line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    model->setData(index, value);
}


void ItemDelegate::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

QVariant ItemDelegate::loadsettings(QString settings) const
{
    QVariant returnvar;
    QSettings appsettings("QTinman","adressbok");
    appsettings.beginGroup(appgroup);
    returnvar = appsettings.value(settings);
    appsettings.endGroup();
    return returnvar;
}
