#ifndef VPKINDEX_H
#define VPKINDEX_H

#include "file-formats_global.h"
#include <QHash>
#include "vpkindextreerecord.h"
#include <QSharedPointer>

namespace FileFormats
{
    typedef QSharedPointer<VPKIndexTreeRecord> VPKIndexTreeRecordPointer;

    class VPKIndex
    {
    public:
        VPKIndex();
        void clear();

        void addRecord(const VPKIndexTreeRecordPointer& record);
        VPKIndexTreeRecordPointer recordAt(const QString& path) const;

        QList<VPKIndexTreeRecordPointer> recordsByExtension(const QString& extension) const;

    private:
        QHash<QString, VPKIndexTreeRecordPointer> m_Records;
        QMultiHash<QString, VPKIndexTreeRecordPointer> m_RecordsByFileExtension;
    };
}

#endif // VPKINDEX_H
