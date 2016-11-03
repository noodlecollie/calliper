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

    QList<VPKIndexTreeRecordPointer> VPKIndex::recordsByExtension(const QString &extension) const
    {
        return m_RecordsByFileExtension.values(extension);
    }

    void VPKIndex::clear()
    {
        m_RecordsByFileExtension.clear();
        m_Records.clear();
    }
}
