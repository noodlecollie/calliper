#include "app-calliper/widgets/mainwindow/mainwindow.h"
#include <QApplication>
#include "application/applicationtasks.h"
#include <QSurfaceFormat>

void initOpenGL()
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(2);
    format.setDepthBufferSize(24);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(0);   // The window surface itself doesn't need any alpha component.
    format.setStencilBufferSize(8);

    QSurfaceFormat::setDefaultFormat(format);
}

int main(int argc, char *argv[])
{
    using namespace AppCalliper;

    initOpenGL();

    QApplication a(argc, argv);
    a.setApplicationName("Calliper");
    a.setApplicationDisplayName("Calliper");
    a.setOrganizationName("Infra");
    a.setOrganizationName("Infra");

    // TODO: Set up an OpenGL context we can use to initialise stuff.

    ApplicationTasks::initSubSystems();

    MainWindow w;
    w.show();

    int returnVal = a.exec();

    ApplicationTasks::shutDownSubSystems();

    return returnVal;
}
