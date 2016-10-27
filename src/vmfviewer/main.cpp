#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("vmfviewer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("VMF Viewer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "VMF file to read");
    parser.process(a);

    QStringList cmdArgs = parser.positionalArguments();
    QString filename;
    if ( !cmdArgs.isEmpty() )
    {
        filename = cmdArgs.at(0);
    }

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

    MainWindow w(filename);
    w.show();

    return a.exec();
}
