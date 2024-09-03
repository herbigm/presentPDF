#include "presentercontrol.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PresenterControl w;

    w.showMaximized();
    return a.exec();
}
