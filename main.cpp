#include "mainwindow.h"
#include "globalshortcut.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    GlobalShortCut *shortcut = new GlobalShortCut("Ctrl+LEFT",&w);
    QObject::connect(shortcut,SIGNAL(activated()),&w,SLOT(activated()));
    return a.exec();
}
