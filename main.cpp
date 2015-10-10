#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include "cviewport.h"
#include <QOpenGLFunctions>

#include <QVector3D>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(2);

    QSurfaceFormat::setDefaultFormat(format);

    qDebug() << "Forward:" << QVector3D(1,0,0) << "Up:" << QVector3D(0,0,1)
             << "FxU:" << QVector3D::crossProduct(QVector3D(1,0,0), QVector3D(0,0,1))
             << "UxF:" << QVector3D::crossProduct(QVector3D(0,0,1), QVector3D(1,0,0));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
