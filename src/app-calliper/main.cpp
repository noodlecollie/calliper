#include "widgets/mainwindow.h"
#include <QApplication>
#include "application/applicationtasks.h"

int main(int argc, char *argv[])
{
    using namespace AppCalliper;

    QApplication a(argc, argv);
    ApplicationTasks::initSubSystems();

    MainWindow w;
    w.show();

    int returnVal = a.exec();

    ApplicationTasks::shutDownSubSystems();

    return returnVal;
}
