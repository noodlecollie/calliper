#if 0
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

    format.setDepthBufferSize(24);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(8);
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

#else

#include <QFile>
#include <QByteArray>
#include "objfileparser.h"
#include <QtDebug>
#include <QList>
#include <QVector3D>
#include <QVector2D>

int main(int argc, char *argv[])
{
    QFile camera(":/models/camera.obj");
    if ( !camera.open(QIODevice::ReadOnly) )
    {
        qDebug() << "Unable to open file.";
        return 1;
    }

    QByteArray arr = camera.readAll();
    QList<QVector3D> positions;
    QList<QVector3D> normals;
    QList<QVector2D> uvs;
    QList<unsigned int> indices;
    ObjFileParser::fillAttributes(arr, positions, normals, uvs, indices);
    camera.close();
}

#endif
