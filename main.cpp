#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QtDebug>
#include "resourcemanager.h"
#include "openglrenderer.h"
#include "application.h"

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

    // Today I learned: the surface itself doesn't need an alpha channel.
    // When we blend colours in OpenGL, the alpha value passed in with the colour
    // is only used internally by OpenGL in order to calculate the physical RGB
    // to write back into the colour buffer. We don't need to set any alpha on the
    // surface itself, since the RGB values will already have been blended by the
    // time they're written back to the colour buffer. If we do set the alpha, that
    // means we'll actually be able to see through the surface and whatever is behind
    // will be visible. This isn't what we want for the application main window!
    // If we want to create a frame buffer later on where the actual colour buffer
    // output should be translucent, override the default alpha buffer size to 8 bits.

    format.setDepthBufferSize(24);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(0);
    format.setStencilBufferSize(8);

    QSurfaceFormat::setDefaultFormat(format);
    qDebug() << "Set default OpenGL format:" << format;

    QApplication a(argc, argv);
    a.setApplicationName("Calliper");
    a.setApplicationDisplayName("Calliper");
    a.setOrganizationName("Infra");
    a.setOrganizationName("Infra");

    // Initialise the resource manager.
    ResourceManager::initialise();

    // Initialise the renderer.
    OpenGLRenderer::initialise();

    // Initialise the over-arching application.
    Application::initialise(new MainWindow());

    // Set up resources.
    resourceManager()->makeCurrent();
    resourceManager()->setUpOpenGLResources();
    renderer()->setUpOpenGLResources();
    resourceManager()->doneCurrent();

    application()->mainWindow()->show();
    
    int ret = a.exec();

    // Shut down the application.
    Application::shutdown();

    // Shut down the renderer.
    OpenGLRenderer::shutdown();

    // Shut down the resource manager.
    ResourceManager::shutdown();

    return ret;
}
