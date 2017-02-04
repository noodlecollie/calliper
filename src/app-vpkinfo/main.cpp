#include <QCoreApplication>
#include <QCommandLineParser>
#include <QtDebug>
#include <QFile>
#include "vpkinfo.h"
#include "file-formats/vpk/vpkfile.h"
#include <iostream>

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

    QCommandLineOption optHeader(QStringList() << "d" << "header", "Output header information.");
    parser.addOption(optHeader);

    QCommandLineOption optIndex(QStringList() << "i" << "index", "Output index information.");
    parser.addOption(optIndex);

    QCommandLineOption optArchiveMD5(QStringList() << "r" << "archive-md5", "Verify archive MD5 checksums.");
    parser.addOption(optArchiveMD5);
    QCommandLineOption optArchiveMD5Verbose(QStringList() << "md5-verbose",
                                            "Output verbose information when verifying archive MD5 checksums.");
    parser.addOption(optArchiveMD5Verbose);

    QCommandLineOption optOtherMD5(QStringList() << "o" << "other-md5", "Output other MD5 information.");
    parser.addOption(optOtherMD5);

    QCommandLineOption optListFiles(QStringList() << "l" << "list-files", "List all files within VPK.");
    parser.addOption(optListFiles);

    QCommandLineOption optFileInfo(QStringList() << "info", "List information about a given file.", "infofile");
    parser.addOption(optFileInfo);

    QCommandLineOption optOutputFile(QStringList() << "output-path", "If info file specified, output this file to the given path", "outpath");
    parser.addOption(optOutputFile);

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

    // TODO: Output error if no options are set.
    bool outputHeader = parser.isSet(optHeader);
    bool outputIndex = parser.isSet(optIndex);
    bool outputArchiveMD5 = parser.isSet(optArchiveMD5) || parser.isSet(optArchiveMD5Verbose);
    bool outputOtherMD5 = parser.isSet(optOtherMD5);
    bool listFiles = parser.isSet(optListFiles);
    bool printIndividualFileInformation = parser.isSet(optFileInfo);

    if ( outputArchiveMD5 )
    {
        if ( !tryReadFile(vpkFile, &FileFormats::VPKFile::readArchiveMD5) )
        {
            return 1;
        }
    }

    QByteArray treeData;
    QByteArray archiveMD5Data;

    if ( outputOtherMD5 )
    {
        if ( !tryReadFile(vpkFile, &FileFormats::VPKFile::readOtherMD5) )
        {
            return 1;
        }

        treeData = vpkFile.treeData();
        archiveMD5Data = vpkFile.archiveMD5Data();
    }

    vpkFile.close();

    if ( outputHeader )
        VPKInfo::printHeaderData(vpkFile.header(), vpkFile.siblingArchives());

    if ( outputIndex )
        VPKInfo::printIndexData(vpkFile.index());

    if ( listFiles )
        VPKInfo::listFiles(vpkFile.index());

    if ( outputArchiveMD5 )
        VPKInfo::printArchiveMD5Data(vpkFile.archiveMD5Collection(), vpkFile.siblingArchives(), parser.isSet(optArchiveMD5Verbose));

    if ( outputOtherMD5 )
        VPKInfo::printOtherMD5Data(vpkFile.otherMD5s(), treeData, archiveMD5Data);

    if ( printIndividualFileInformation )
    {
        VPKInfo::printInfoAboutFile(vpkFile.index(), parser.value(optFileInfo));
        QString outPath = parser.value(optOutputFile);
        if ( !outPath.isNull() )
        {
            VPKInfo::outputFile(vpkFile, parser.value(optFileInfo), outPath);
        }
    }

    return 0;
}
