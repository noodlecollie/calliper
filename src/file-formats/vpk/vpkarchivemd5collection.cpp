#include "vpkarchivemd5collection.h"

namespace FileFormats
{
    QSet<quint32> VPKArchiveMD5Collection::archiveIndices() const
    {
        QSet<quint32> indices;

        foreach ( const VPKArchiveMD5ItemPointer& item, m_Items )
        {
            indices.insert(item->archiveIndex());
        }

        return indices;
    }
}
