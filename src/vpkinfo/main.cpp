#include <QCoreApplication>
#include <QCommandLineParser>
#include <QtDebug>
#include <QFile>
#include "vpkinfo.h"
#include "vpk/vpkfile.h"

namespace
{
    typedef bool (FileFormats::VPKFile::* VPKReadFunc)(QString*);

    bool tryReadFile(FileFormats::VPKFile& file, VPKReadFunc function)
    {
        QString error;
        if ( !(file.*function)(&error) )
        {
            qCritical() << "Error reading file:" << error;
            return false;
        }

        return true;
    }
}

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

    QCommandLineOption optArchiveMD5(QStringList() << "r" << "archive-md5", "Output archive MD5 information.");
    parser.addOption(optArchiveMD5);

    QCommandLineOption optOtherMD5(QStringList() << "o" << "other-md5", "Output other MD5 information.");
    parser.addOption(optOtherMD5);

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

    if ( !tryReadFile(vpkFile, &FileFormats::VPKFile::readIndex) )
    {
        return 1;
    }

    bool outputAll = parser.isSet(optAll) ||
            (!parser.isSet(optHeader) &&
             !parser.isSet(optIndex) &&
             !parser.isSet(optArchiveMD5) &&
             !parser.isSet(optOtherMD5));

    bool outputHeader = outputAll || parser.isSet(optHeader);
    bool outputIndex = outputAll || parser.isSet(optIndex);
    bool outputArchiveMD5 = outputAll || parser.isSet(optArchiveMD5);
    bool outputOtherMD5 = outputAll || parser.isSet(optOtherMD5);

    if ( outputArchiveMD5 )
    {
        if ( !tryReadFile(vpkFile, &FileFormats::VPKFile::readArchiveMD5) )
        {
            return 1;
        }
    }

    if ( outputOtherMD5 )
    {
        if ( !tryReadFile(vpkFile, &FileFormats::VPKFile::readOtherMD5) )
        {
            return 1;
        }
    }

    vpkFile.close();

    if ( outputHeader )
        VPKInfo::printHeaderData(vpkFile.header(), vpkFile.siblingArchives());

    if ( outputIndex )
        VPKInfo::printIndexData(vpkFile.index());

    if ( outputArchiveMD5 )
        VPKInfo::printArchiveMD5Data(vpkFile.archiveMD5Collection());

    if ( outputOtherMD5 )
        VPKInfo::printOtherMD5Data(vpkFile.otherMD5Collection());

    return 0;
}
