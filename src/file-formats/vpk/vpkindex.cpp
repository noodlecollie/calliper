#include "vpkindex.h"

namespace FileFormats
{
    VPKIndex::VPKIndex()
    {

    }

    void VPKIndex::addRecord(const VPKIndexTreeRecordPointer &record)
    {
        if ( m_Records.contains(record->fullPath()) )
            return;

        m_Records.insert(record->fullPath(), record);
        m_RecordsByFileExtension.insertMulti(record->extension(), record);
    }

    VPKIndexTreeRecordPointer VPKIndex::recordAt(const QString &path) const
    {
        return m_Records.value(path, VPKIndexTreeRecordPointer());
    }

    QList<VPKIndexTreeRecordPointer> VPKIndex::recordsForExtension(const QString &extension) const
    {
        return m_RecordsByFileExtension.values(extension);
    }

    int VPKIndex::recordCountForExtension(const QString &extension) const
    {
        return m_RecordsByFileExtension.count(extension);
    }

    QStringList VPKIndex::extensions() const
    {
        return m_RecordsByFileExtension.uniqueKeys();
    }

    int VPKIndex::recordCount() const
    {
        return m_Records.count();
    }

    void VPKIndex::clear()
    {
        m_RecordsByFileExtension.clear();
        m_Records.clear();
    }
}
