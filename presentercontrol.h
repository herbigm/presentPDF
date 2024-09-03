#ifndef PRESENTERCONTROL_H
#define PRESENTERCONTROL_H

#include <QMainWindow>
#include <poppler-qt5.h>
#include <QDebug>
#include <QWindow>
#include <QScreen>
#include <QShortcut>
#include <QFileDialog>
#include <QKeyEvent>
#include <QTimer>
#include <QTranslator>
#include <QMessageBox>

#include "pdfwidget.h"
#include "presentationwidget.h"
#include "thumbnailpdfwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PresenterControl; }
QT_END_NAMESPACE

class PresenterControl : public QMainWindow
{
    Q_OBJECT

public:
    PresenterControl(QWidget *parent = nullptr);
    ~PresenterControl();

private slots:
    void stopPresenting();

    void on_actionQuit_triggered();

    void on_actionLoad_PDF_triggered();

    void showPage();
    void nextPage();
    void previousPage();

    void on_startPresentationButton_clicked();

    void on_startPresentationAtButton_clicked();

    void on_stopPresentationButton_clicked();

    void on_navNextButton_clicked();

    void on_navPreviousButton_clicked();

    void thumbClick(int idx);

    void thumbDblClick(int idx);

    void updateTime();

    void on_actionabout_PresentPDF_triggered();

    void on_actionabout_Qt_triggered();

private:
    Ui::PresenterControl *ui;
    PresentationWidget *presentationWidget;
    Poppler::Document *document;
    int currentPage;
    QVector<QImage> thumbnails;
    QVector<QPixmap> gross;

    // shortcuts
    QShortcut *shortcutQuit;
    QShortcut *shortcutNext;
    QShortcut *shortcutPrevious;
    QTime startTime;
    QTimer timer;

    QString inputUntilNow;

protected:
     void closeEvent(QCloseEvent *event);
     void keyPressEvent(QKeyEvent *event);
};
#endif // PRESENTERCONTROL_H
