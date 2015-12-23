#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QtDebug>
#include "resourcemanager.h"

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

    // Initialise the resource manager.
    ResourceManager::initialise();

    MainWindow w;
    w.show();
    
    int ret = a.exec();

    // Shut down the resource manager.
    ResourceManager::shutdown();

    return ret;
}
