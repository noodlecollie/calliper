#include "widgets/mainwindow.h"
#include <QApplication>
#include "application/applicationtasks.h"

int main(int argc, char *argv[])
{
    using namespace AppCalliper;

    QApplication a(argc, argv);

    // TODO: Set up an OpenGL context we can use to initialise stuff.

    ApplicationTasks::initSubSystems();

    MainWindow w;
    w.show();

    int returnVal = a.exec();

    ApplicationTasks::shutDownSubSystems();

    return returnVal;
}
