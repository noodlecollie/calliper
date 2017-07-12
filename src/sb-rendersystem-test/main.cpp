#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

#include "rendersystem/global/rendersystem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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

    RenderSystem::Global::initialise();

    MainWindow* w = new MainWindow;
    w->show();

    int ret = a.exec();

    delete w;
    w = Q_NULLPTR;

    RenderSystem::Global::shutdown();

    return ret;
}