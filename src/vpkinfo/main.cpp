#include <QCoreApplication>
#include <QCommandLineParser>
#include <QtDebug>
#include <QFile>
#include "vpkinfo.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("vpkinfo");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("VPK Info");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption optAll(QStringList() << "a" << "all", "Outputs all information. Overrides other flags. "
                              "If no flags are set, all information is output by default.");
    parser.addOption(optAll);

    QCommandLineOption optHeader(QStringList() << "h" << "header", "Output header information.");
    parser.addOption(optHeader);

    parser.addPositionalArgument("file", "VPK file to read");

    parser.process(a);

    QStringList cmdArgs = parser.positionalArguments();
    QString filename;
    if ( !cmdArgs.isEmpty() )
    {
        filename = cmdArgs.at(0);
    }

    if ( filename.isEmpty() )
    {
        qCritical() << "No VPK file provided.";
        return 1;
    }

    QFile file(filename);
    if ( !file.open(QIODevice::ReadOnly) )
    {
        qCritical() << "Unable to open file" << filename;
        return 1;
    }

    qDebug() << "Reading file" << filename;

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    VPKInfo vpk;
    vpk.read(in);
    if ( !vpk.errorString().isEmpty() )
    {
        qDebug() << "Error reading VPK:" << vpk.errorString();
        return 1;
    }

    bool outputAll = parser.isSet(optAll) ||
            (!parser.isSet(optHeader) /*&& !others*/);

    if ( outputAll || parser.isSet(optHeader) )
        vpk.printHeaderData();

    return 0;
}
