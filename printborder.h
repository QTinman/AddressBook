#ifndef PRINTBORDER_H
#define PRINTBORDER_H
#include <QObject>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include "tableprinter.h"


class PrintBorder : public PagePrepare {
public:
    virtual void preparePage(QPainter *painter);
    static int pageNumber;
    QVariant loadsettings(QString settings);
    void savesettings(QString settings, QVariant attr);
};

#endif // PRINTBORDER_H
