#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("vmfviewer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("VMF Viewer");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption opVpkPath("vpkpath", "Path where VPK content is stored.", "path");
    parser.addOption(opVpkPath);

    parser.addPositionalArgument("file", "VMF file to read.");

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

    MainWindow w;
    w.setMapPath(filename);
    w.setVpkPath(parser.value(opVpkPath));
    w.show();
    w.importTextures();
    w.loadMap();

    return a.exec();
}
