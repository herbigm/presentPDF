#include "presentationwidget.h"

PresentationWidget::PresentationWidget(QWidget *parent):
    PDFWidget(parent)
{
    // define shortcuts

}

void PresentationWidget::stopPresenting()
{
    this->hide();
    qDebug() << "a";
}
