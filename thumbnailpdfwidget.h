#ifndef THUMBNAILPDFWIDGET_H
#define THUMBNAILPDFWIDGET_H

#include <QObject>
#include <QWidget>

#include "pdfwidget.h"

class ThumbnailPDFWidget : public PDFWidget
{
    Q_OBJECT
public:
    ThumbnailPDFWidget(QWidget *parent = nullptr, int idx = -1);
    ~ThumbnailPDFWidget();

signals:
    void ThumbnailClicked(int idx);
    void ThumbnailDoubleClicked(int idx);

private:
    int index;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
};

#endif // THUMBNAILPDFWIDGET_H
