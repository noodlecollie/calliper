#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "model/global/resourceenvironment.h"
#include "renderer/global/openglbackgroundcontext.h"
#include "renderer/opengl/scopedcurrentcontext.h"
#include "calliperutil/debug/debug.h"

void initSubSystems()
{
    Renderer::OpenGLBackgroundContext::globalInitialise();
    CUTL_ASSERT_SUCCESS(Renderer::OpenGLBackgroundContext::globalInstance()->create());

//    Renderer::ScopedCurrentContext scopedContext;
//    Q_UNUSED(scopedContext);

//    Model::ResourceEnvironment::globalInitialise();
}

void shutDownSubSystems()
{
//    {
//        Renderer::ScopedCurrentContext scopedContext;
//        Q_UNUSED(scopedContext);

//        Model::ResourceEnvironment::globalShutdown();
//    }

    Renderer::OpenGLBackgroundContext::globalShutdown();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("vmfviewer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("VMF Viewer");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption opVpkPath("vpkpath", "Folder where VPK content is stored. There is no need to specify the actual VPK files.", "path");
    parser.addOption(opVpkPath);

    parser.addPositionalArgument("file", "VMF file to read.");

    parser.setApplicationDescription(
                "Simple viewer for VMF files. Provide a path to the folder where "
                "VPKs are located with --vpkpath; the first non-named argument is "
                "interpreted as the VMF to load.");

    parser.addHelpOption();

    parser.process(a);

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
    format.setAlphaBufferSize(0);
    format.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);

    QString filename;
    QStringList opts = parser.positionalArguments();
    if ( !opts.isEmpty() )
    {
        filename = opts.at(0);
    }

    initSubSystems();

    MainWindow* w = new MainWindow();
    w->setMapPath(filename);
    w->setVpkPath(parser.value(opVpkPath));
    w->show();

    int ret = a.exec();

    shutDownSubSystems();

    return ret;
}
