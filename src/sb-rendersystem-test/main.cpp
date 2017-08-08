#include "mainwindow.h"
#include "mainwindow2.h"

#include <QApplication>
#include <QSurfaceFormat>

#define MAINWIN_VERSION 2

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(1);
    format.setDepthBufferSize(24);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(0);   // The window surface itself doesn't need any alpha component.
    format.setStencilBufferSize(8);

    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);

#if MAINWIN_VERSION == 1
    MainWindow* w = new MainWindow();
#elif MAINWIN_VERSION == 2
    MainWindow2* w = new MainWindow2();
#endif

    w->show();

    int ret = a.exec();

    delete w;
    w = Q_NULLPTR;

    return ret;
}
