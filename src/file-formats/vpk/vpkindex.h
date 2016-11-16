#ifndef VPKINDEX_H
#define VPKINDEX_H

#include "file-formats_global.h"
#include <QHash>
#include <QMap>
#include "vpkindextreerecord.h"
#include <QSharedPointer>

namespace FileFormats
{
    typedef QSharedPointer<VPKIndexTreeRecord> VPKIndexTreeRecordPointer;

    class FILEFORMATSSHARED_EXPORT VPKIndex
    {
    public:
        VPKIndex();
        void clear();

        void addRecord(const VPKIndexTreeRecordPointer& record);
        VPKIndexTreeRecordPointer recordAt(const QString& path) const;
        int recordCount() const;
        QStringList recordPaths() const;

        QList<VPKIndexTreeRecordPointer> recordsForExtension(const QString& extension) const;
        int recordCountForExtension(const QString& extension) const;
        QStringList extensions() const;

    private:
        QHash<QString, VPKIndexTreeRecordPointer> m_Records;
        QMultiMap<QString, VPKIndexTreeRecordPointer> m_RecordsByFileExtension;
    };
}

#endif // VPKINDEX_H
