#include "thumbnailpdfwidget.h"

ThumbnailPDFWidget::ThumbnailPDFWidget(QWidget *parent, int idx):
    PDFWidget(parent),
    index(idx)
{

}

ThumbnailPDFWidget::~ThumbnailPDFWidget()
{

}

void ThumbnailPDFWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit ThumbnailClicked(index);
}

void ThumbnailPDFWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit ThumbnailDoubleClicked(index);
}
