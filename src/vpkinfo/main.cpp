#include <QCoreApplication>
#include <QCommandLineParser>
#include <QtDebug>
#include <QFile>
#include "vpkinfo.h"
#include "vpk/vpkfile.h"

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

    QCommandLineOption optIndex(QStringList() << "i" << "index", "Output index information.");
    parser.addOption(optIndex);

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

    qDebug() << "Reading file" << filename;

    FileFormats::VPKFile vpkFile(filename);

    if ( !vpkFile.open() )
    {
        qCritical() << "Unable to open file" << filename;
        return 1;
    }

    QString error;
    if ( !vpkFile.readIndex(&error) ||
         !vpkFile.readArchiveMD5(&error) ||
         !vpkFile.readOtherMD5(&error) )
    {
        qDebug() << "Error reading file:" << error;
        return 1;
    }

    vpkFile.close();

    bool outputAll = parser.isSet(optAll) ||
            (!parser.isSet(optHeader) &&
             !parser.isSet(optIndex) );

    if ( outputAll || parser.isSet(optHeader) )
        VPKInfo::printHeaderData(vpkFile.header(), vpkFile.siblingArchives());

    if ( outputAll || parser.isSet(optIndex) )
        VPKInfo::printIndexData(vpkFile.index());

    return 0;
}
