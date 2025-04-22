#include "currentmonitorfirstwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    currentMonitorFirstWindow w;
    w.show();
    return a.exec();
}
