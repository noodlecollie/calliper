#ifndef VPKINDEXTREERECORD_H
#define VPKINDEXTREERECORD_H

#include "file-formats_global.h"
#include "vpkindextreeitem.h"
#include <QScopedPointer>

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKIndexTreeRecord
    {
    public:
        VPKIndexTreeRecord(const QString& path, const QString& filename, const QString& extension);

        QString fullPath() const;
        VPKIndexTreeItem* item() const;

        QString path() const;
        QString filename() const;
        QString extension() const;

    private:
        QString m_strPath;
        QString m_strFilename;
        QString m_strExtension;
        QScopedPointer<VPKIndexTreeItem> m_pItem;
    };
}

#endif // VPKINDEXTREERECORD_H
