#include "vpkindextreerecord.h"

namespace FileFormats
{
    VPKIndexTreeRecord::VPKIndexTreeRecord(const QString &path, const QString &filename, const QString &extension)
        : m_strPath(path), m_strFilename(filename), m_strExtension(extension),
          m_pItem(new VPKIndexTreeItem())
    {

    }

    QString VPKIndexTreeRecord::fullPath() const
    {
        return QString("%1/%2.%3")
                .arg(m_strPath)
                .arg(m_strFilename)
                .arg(m_strExtension);
    }

    VPKIndexTreeItem* VPKIndexTreeRecord::item() const
    {
        return m_pItem.data();
    }

    QString VPKIndexTreeRecord::path() const
    {
        return m_strPath;
    }

    QString VPKIndexTreeRecord::filename() const
    {
        return m_strFilename;
    }

    QString VPKIndexTreeRecord::extension() const
    {
        return m_strExtension;
    }
}
