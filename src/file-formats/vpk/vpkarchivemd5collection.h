#ifndef VPKARCHIVEMD5COLLECTION_H
#define VPKARCHIVEMD5COLLECTION_H

#include "file-formats_global.h"
#include "collection/simpleitemcollection.h"
#include "vpkarchivemd5item.h"
#include <QSet>

namespace FileFormats
{
    typedef QSharedPointer<VPKArchiveMD5Item> VPKArchiveMD5ItemPointer;

    class FILEFORMATSSHARED_EXPORT VPKArchiveMD5Collection : public SimpleItemCollection<VPKArchiveMD5Item>
    {
    public:
        VPKArchiveMD5Collection() : SimpleItemCollection<VPKArchiveMD5Item>()
        {
        }

        QSet<quint32> archiveIndices() const;
    };
}

#endif // VPKARCHIVEMD5COLLECTION_H
