#include "host_switcher.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    HostSwitcher w;
    w.show();
    return a.exec();
}
