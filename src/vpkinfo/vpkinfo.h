#ifndef VPKINFO_H
#define VPKINFO_H

#include <QDataStream>
#include "vpk/vpkheader.h"
#include "vpk/vpkindex.h"
#include "vpk/vpkarchivemd5collection.h"
#include "vpk/vpkothermd5collection.h"
#include <QStringList>

namespace VPKInfo
{
    void printHeaderData(const FileFormats::VPKHeader& header, const QStringList& siblingArchives);
    void printIndexData(const FileFormats::VPKIndex& index);
    void printArchiveMD5Data(const FileFormats::VPKArchiveMD5Collection& collection, const QStringList& archives);
    void printOtherMD5Data(const FileFormats::VPKOtherMD5Collection& collection);
}

#endif // VPKINFO_H
