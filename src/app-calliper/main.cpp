#include "widgets/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    using namespace AppCalliper;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
