#ifndef VPKARCHIVEMD5COLLECTION_H
#define VPKARCHIVEMD5COLLECTION_H

#include "file-formats_global.h"
#include "vpkarchivemd5item.h"
#include <QSet>
#include <QSharedPointer>

namespace FileFormats
{
    typedef QSharedPointer<VPKArchiveMD5Item> VPKArchiveMD5ItemPointer;

    class FILEFORMATSSHARED_EXPORT VPKArchiveMD5Collection
    {
    public:
        VPKArchiveMD5Collection();

        void addItem(const VPKArchiveMD5ItemPointer& item);
        VPKArchiveMD5ItemPointer itemAt(int index) const;
        int count() const;
        void clear();

        QSet<quint32> archiveIndices() const;

    private:
        QList<VPKArchiveMD5ItemPointer> m_Items;
    };
}

#endif // VPKARCHIVEMD5COLLECTION_H
