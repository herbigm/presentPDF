#include "presentercontrol.h"
#include "ui_presentercontrol.h"

PresenterControl::PresenterControl(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PresenterControl)
{
    ui->setupUi(this);
    presentationWidget = new PresentationWidget();

    QTranslator myappTranslator;
    myappTranslator.load(":/languages/PresentPDF_" + QLocale::system().name() + ".qm");
    qApp->installTranslator(&myappTranslator);
    ui->retranslateUi(this);

    document = nullptr;

    currentPage = -1;

    // define shortcuts
    shortcutQuit = new QShortcut(Qt::Key_Escape, this);
    shortcutQuit->setContext(Qt::ApplicationShortcut);
    connect(shortcutQuit, &QShortcut::activated, this, &PresenterControl::stopPresenting);

    shortcutNext = new QShortcut(Qt::Key_Right, this);
    shortcutNext->setContext(Qt::ApplicationShortcut);
    connect(shortcutNext, &QShortcut::activated, this, &PresenterControl::nextPage);

    shortcutPrevious = new QShortcut(Qt::Key_Left, this);
    shortcutPrevious->setContext(Qt::ApplicationShortcut);
    connect(shortcutPrevious, &QShortcut::activated, this, &PresenterControl::previousPage);

    this->setBackgroundRole(QPalette::Shadow);
    this->setForegroundRole(QPalette::Dark);
    this->setAutoFillBackground(true);

    QPalette palette;
    palette.setBrush(QPalette::Base, Qt::transparent);
    ui->TimerBox->setPalette(palette);

    // timer
    connect(&timer, &QTimer::timeout, this, &PresenterControl::updateTime);

    // default cleaning
    ui->previousPagePDF->clear();
    ui->currentPagePDF->clear();
    ui->nextPagePDF->clear();
}

PresenterControl::~PresenterControl()
{
    delete presentationWidget;
    delete ui;
}

void PresenterControl::stopPresenting()
{
    presentationWidget->hide();
    timer.stop();
}

void PresenterControl::closeEvent(QCloseEvent *event)
{
    presentationWidget->hide();
    QMainWindow::closeEvent(event);
}

void PresenterControl::keyPressEvent(QKeyEvent *event)
{
    // presenter keys
    if(event->key() == 16777238) {
        previousPage();
    }

    if(event->key() == 16777239) {
        nextPage();
    }

    // Jump to a page
    if(event->key() <= 57 && event->key() >= 48) {
        inputUntilNow += event->text();
    }

    if(event->key() == 16777220) { // enter
        int page = inputUntilNow.toInt();
        if(page < document->numPages()+1 && page >= 0) {
            currentPage = page-1;
            showPage();
        }
        inputUntilNow.clear();
    }
}

void PresenterControl::on_actionQuit_triggered()
{
    QApplication::quit();
}

void PresenterControl::on_actionLoad_PDF_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open PDF"), QDir::homePath(), tr("PDF files (*.pdf)"));
    if(!fileName.isEmpty()) {

        // default cleaning
        ui->previousPagePDF->clear();
        ui->currentPagePDF->clear();
        ui->nextPagePDF->clear();
        thumbnails.clear();
        gross.clear();

        // if an old file is loaded, unload
        while(ui->ScrollAreaLayout->count() > 0) {
            QLayoutItem *i = ui->ScrollAreaLayout->takeAt(0);
            i->widget()->deleteLater();
        }

        // load the file
        document = Poppler::Document::load(fileName);
        if (!document || document->isLocked()) {
            qDebug() << tr("PDF document not valid.");
            return;
        }

        // Paranoid safety check
        if (document == 0) {
            qDebug() << tr("PDF document not valid.");
            return;
        }
        // load pages later
        ui->currentPagePDF->loadImage(document->page(0)->renderToImage(72,72));
        if(document->numPages() > 1) {
            ui->nextPagePDF->loadImage(document->page(1)->renderToImage(72,72));
        }

        currentPage = 0;

        //test
        QScreen *sc= QGuiApplication::screens()[1];
        int width = sc->geometry().width();
        int height = sc->geometry().height();

        for (int i = 0; i < document->numPages(); i++) {
            thumbnails.append(document->page(i)->renderToImage());
            gross.append(QPixmap::fromImage(document->page(i)->renderToImage(600,600)).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ThumbnailPDFWidget *w = new ThumbnailPDFWidget(reinterpret_cast<QWidget *>(ui->thumbnailScrollArea), i);
            w->setGeometry(0,0,200,200);
            w->loadImage(thumbnails.last());
            ui->ScrollAreaLayout->addWidget(w);
            connect(w, &ThumbnailPDFWidget::ThumbnailClicked, this, &PresenterControl::thumbClick);
            connect(w, &ThumbnailPDFWidget::ThumbnailDoubleClicked, this, &PresenterControl::thumbDblClick);
        }
    }
}

void PresenterControl::showPage()
{
    presentationWidget->loadPixmap(gross.at(currentPage));
    ui->currentPagePDF->loadImage(thumbnails.at(currentPage));
    thumbClick(currentPage);
    ui->thumbnailScrollArea->ensureWidgetVisible(ui->ScrollAreaLayout->itemAt(currentPage)->widget());

    if(currentPage < document->numPages()-1) {
        ui->nextPagePDF->loadImage(thumbnails.at(currentPage+1));
    } else {
        ui->nextPagePDF->clear();
    }
    if(currentPage > 0) {
        ui->previousPagePDF->loadImage(thumbnails.at(currentPage-1));
    } else {
        ui->previousPagePDF->clear();
    }
}

void PresenterControl::nextPage()
{
    if(document != nullptr) {
        currentPage += 1;
        if(currentPage >= document->numPages()) {
            currentPage = document->numPages()-1;
        } else if(currentPage < 0){
            currentPage = 0;
        }
        showPage();
    }
}

void PresenterControl::previousPage()
{
    if(document != nullptr) {
        currentPage -= 1;
        if(currentPage >= document->numPages()) {
            currentPage = document->numPages()-1;
        } else if(currentPage < 0){
            currentPage = 0;
        }
        showPage();
    }
}

void PresenterControl::on_startPresentationButton_clicked()
{
    if(document != nullptr) {
        // Access page of the PDF file
        currentPage = 0;

        presentationWidget->show();
        QScreen *sc= QGuiApplication::screens()[1];
        presentationWidget->windowHandle()->setScreen(sc);
        presentationWidget->windowHandle()->setPosition(sc->geometry().topRight());
        presentationWidget->setGeometry(sc->geometry());
        presentationWidget->showFullScreen();
        timer.setInterval(1000);
        timer.start();
        startTime = QTime::currentTime();
        showPage();
    } else {
        statusBar()->showMessage(tr("No document loaded :-("));
    }
}

void PresenterControl::on_startPresentationAtButton_clicked()
{
    if(document != nullptr) {
        // Access page of the PDF file
        presentationWidget->show();
        QScreen *sc= QGuiApplication::screens()[1];
        presentationWidget->windowHandle()->setScreen(sc);
        presentationWidget->windowHandle()->setPosition(sc->geometry().topRight());
        presentationWidget->setGeometry(sc->geometry());
        presentationWidget->showFullScreen();
        timer.setInterval(1000);
        timer.start();
        startTime = QTime::currentTime();
        showPage();
    } else {
        statusBar()->showMessage(tr("No document loaded :-("));
    }
}

void PresenterControl::on_stopPresentationButton_clicked()
{
    stopPresenting();
}

void PresenterControl::on_navNextButton_clicked()
{
    nextPage();
}

void PresenterControl::on_navPreviousButton_clicked()
{
    previousPage();
}

void PresenterControl::thumbClick(int idx)
{
    for(int i = 0; i < ui->ScrollAreaLayout->count(); i++) {
        if(i == idx) {
            ui->ScrollAreaLayout->itemAt(i)->widget()->setStyleSheet("QLabel { border: 5px solid blue; }");
        } else {
            ui->ScrollAreaLayout->itemAt(i)->widget()->setStyleSheet("");
        }
    }
}

void PresenterControl::thumbDblClick(int idx)
{
    currentPage = idx;
    showPage();
}

void PresenterControl::updateTime()
{
    int timeIntervall = startTime.secsTo(QTime::currentTime());
    int maxsecs = ui->maxMinBox->value() * 60;
    if(timeIntervall <= 0.8 * maxsecs) {
        ui->timeLabel->setStyleSheet("QLabel { color: #0064a8; }");
    } else if (timeIntervall > 0.8 * maxsecs && timeIntervall <= maxsecs) {
        ui->timeLabel->setStyleSheet("QLabel { color:  #1a962b; }");
    } else if (timeIntervall <= 1.2 * maxsecs && timeIntervall > maxsecs){
        ui->timeLabel->setStyleSheet("QLabel { color:  #e08603; }");
    } else {
        ui->timeLabel->setStyleSheet("QLabel { color:  #b5123e; }");
    }

    int secs = timeIntervall % 60;
    QString secStr = QString::number(secs);
    if(secs < 10) {
        secStr = "0"+QString::number(secs);
    }
    int mins = timeIntervall/60;
    QString minStr = QString::number(mins);
    if(mins < 10) {
        minStr = "0"+QString::number(mins);
    }
    ui->timeLabel->setText(minStr + ":" + secStr);
}

void PresenterControl::on_actionabout_PresentPDF_triggered()
{
    QMessageBox::about(this, tr("about presentPDF"), tr("This programm is written by Marcus Herbig."));
}

void PresenterControl::on_actionabout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}
