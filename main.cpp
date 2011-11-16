#include "host_switcher.h"

#include <QtGui>
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
#ifdef Q_WS_WIN
        // set text codec
        QTextCodec *gbk = QTextCodec::codecForName("gb18030");
        QTextCodec::setCodecForTr(gbk);
        QTextCodec::setCodecForLocale(gbk);
        QTextCodec::setCodecForCStrings(gbk);

#else
        // set text codec
        QTextCodec *utg8 = QTextCodec::codecForName("utf-8");
        QTextCodec::setCodecForTr(utg8);
        QTextCodec::setCodecForLocale(utg8);
        QTextCodec::setCodecForCStrings(utg8);
#endif

    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    HostSwitcher w;
    w.show();
    return a.exec();
}
