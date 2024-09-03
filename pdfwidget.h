#ifndef PDFWIDGET_H
#define PDFWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPalette>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QPainter>

class PDFWidget : public QLabel
{
    Q_OBJECT
public:
    PDFWidget(QWidget *parent = nullptr);
    ~PDFWidget();

    void loadImage(QImage img);
    void loadPixmap(QPixmap pix);

    double getWidth() { return this->geometry().width(); }
    double getHeight() { return this->geometry().height(); }

private:
    QPalette palette;
};

#endif // PDFWIDGET_H
