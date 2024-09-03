#include "pdfwidget.h"

PDFWidget::PDFWidget(QWidget *parent):
    QLabel(parent)
{
    // set Backgorund
    palette.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    this->setAlignment(Qt::AlignCenter);
}

PDFWidget::~PDFWidget()
{

}

void PDFWidget::loadImage(QImage img)
{
    QPixmap pm;
    pm.convertFromImage(img);
    this->setPixmap(pm.scaled(this->geometry().width(), this->geometry().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void PDFWidget::loadPixmap(QPixmap pix)
{
    this->setPixmap(pix);
}


