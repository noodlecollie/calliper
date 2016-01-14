#if 1

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

#include <QtDebug>

class A { public: A(){} virtual ~A(){} };
class B : public A { public: B() : A() {} virtual ~B(){} };
class C { public: C(){} virtual ~C(){} };
class AC : public A, public C { public: AC() : A(), C() {} virtual ~AC(){} };
class D : public B, public C { public: D() : B(), C() {} virtual ~D(){} };

int main(int argc, char *argv[])
{
    D classD;
    AC* pClassAC = dynamic_cast<AC*>(&classD);
    A* pClassA = dynamic_cast<A*>(&classD);
    C* pClassC = dynamic_cast<C*>(&classD);
    qDebug() << "pClassAC =" << pClassAC
             << "pClassA =" << pClassA
             << "pClassC =" << pClassC;
    return 0;
}

#endif
