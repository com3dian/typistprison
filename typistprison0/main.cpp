#include "mainwindow.h"
#include "customstyle.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(new CustomStyle);
    MainWindow w;
    w.show();
    return a.exec();
}
