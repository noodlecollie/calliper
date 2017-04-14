#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include "renderer/global/mainrendercontext.h"
#include "temprender.h"

int main(int argc, char *argv[])
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

    QApplication a(argc, argv);

    Renderer::MainRenderContext::globalInitialise();
    Renderer::MainRenderContext::globalInstance()->create();
    bool success = Renderer::MainRenderContext::globalInstance()->makeCurrent();
    Q_ASSERT(success);
    tempRenderInstance = new TempRender();
    Renderer::MainRenderContext::globalInstance()->doneCurrent();

    MainWindow* w = new MainWindow();
    w->show();

    int ret = a.exec();

    delete w;
    success = Renderer::MainRenderContext::globalInstance()->makeCurrent();
    Q_ASSERT(success);
    delete tempRenderInstance;
    Renderer::MainRenderContext::globalInstance()->doneCurrent();
    Renderer::MainRenderContext::globalShutdown();
    return ret;
}
