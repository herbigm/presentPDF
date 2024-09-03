#ifndef PRESENTATIONWIDGET_H
#define PRESENTATIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QShortcut>
#include <QDebug>

#include "pdfwidget.h"

class PresentationWidget : public PDFWidget
{
    Q_OBJECT
public:
    PresentationWidget(QWidget *parent = nullptr);
    void stopPresenting();
};

#endif // PRESENTATIONWIDGET_H
