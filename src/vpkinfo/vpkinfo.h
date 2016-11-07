#ifndef VPKINFO_H
#define VPKINFO_H

#include <QDataStream>
#include "vpk/vpkheader.h"
#include "vpk/vpkindex.h"

namespace VPKInfo
{
    void printHeaderData(const FileFormats::VPKHeader& header, const QStringList& siblingArchives);
    void printIndexData(const FileFormats::VPKIndex& index);
}

#endif // VPKINFO_H
