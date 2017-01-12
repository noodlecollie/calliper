#ifndef VPKINDEXTREERECORD_H
#define VPKINDEXTREERECORD_H

#include "file-formats_global.h"
#include "vpkindextreeitem.h"
#include <QScopedPointer>
#include "vpkindextreeiterator.h"

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKIndexTreeRecord
    {
    public:
        VPKIndexTreeRecord(const QString& path, const QString& filename, const QString& extension);
        VPKIndexTreeRecord(const VPKIndexTreeIterator& it);

        QString fullPath() const;
        VPKIndexTreeItem* item() const;

        QString path() const;
        QString fileName() const;
        QString extension() const;

    private:
        QString m_strPath;
        QString m_strFilename;
        QString m_strExtension;
        QScopedPointer<VPKIndexTreeItem> m_pItem;
    };
}

#endif // VPKINDEXTREERECORD_H
