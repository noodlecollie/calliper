#include <QApplication>
#include "demoglwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DemoGLWindow window;
    window.show();

    return a.exec();
}
