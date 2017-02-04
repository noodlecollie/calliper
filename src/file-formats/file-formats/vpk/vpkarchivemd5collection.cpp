#include "vpkarchivemd5collection.h"

namespace FileFormats
{
    VPKArchiveMD5Collection::VPKArchiveMD5Collection()
    {
    }

    void VPKArchiveMD5Collection::addItem(const VPKArchiveMD5ItemPointer &item)
    {
        m_Items.append(item);
    }

    VPKArchiveMD5ItemPointer VPKArchiveMD5Collection::itemAt(int index) const
    {
        return m_Items.at(index);
    }

    int VPKArchiveMD5Collection::count() const
    {
        return m_Items.count();
    }

    void VPKArchiveMD5Collection::clear()
    {
        m_Items.clear();
    }

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
