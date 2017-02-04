#ifndef VPKINFO_H
#define VPKINFO_H

#include <QDataStream>
#include "file-formats/vpk/vpkfile.h"
#include "file-formats/vpk/vpkheader.h"
#include "file-formats/vpk/vpkindex.h"
#include "file-formats/vpk/vpkarchivemd5collection.h"
#include "file-formats/vpk/vpkothermd5item.h"
#include <QStringList>

namespace VPKInfo
{
    void printHeaderData(const FileFormats::VPKHeader& header, const QStringList& siblingArchives);
    void printIndexData(const FileFormats::VPKIndex& index);
    void listFiles(const FileFormats::VPKIndex& index);
    void printArchiveMD5Data(const FileFormats::VPKArchiveMD5Collection& collection,
                             const QStringList& archives, bool verbose = false);
    void printOtherMD5Data(const FileFormats::VPKOtherMD5Item& md5s,
                           const QByteArray& treeData, const QByteArray& archiveMD5Data);
    void printInfoAboutFile(const FileFormats::VPKIndex& index, const QString& filename);
    void outputFile(FileFormats::VPKFile& file, const QString& filePath, const QString& outputPath);
}

#endif // VPKINFO_H
