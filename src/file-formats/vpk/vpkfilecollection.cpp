#include "vpkfilecollection.h"

namespace FileFormats
{
    VPKFileCollection::VPKFileCollection()
    {

    }

    VPKFileCollection::~VPKFileCollection()
    {
        qDeleteAll(m_FilesContainingExtensions.values());
        m_FilesContainingExtensions.clear();
    }

    void VPKFileCollection::addFile(const VPKFilePointer &file)
    {
        m_Files.append(file);
        processAddFile(file);
    }

    void VPKFileCollection::processAddFile(const VPKFilePointer &file)
    {
        QStringList extensions = file->index().extensions();

        foreach ( const QString& ext, extensions )
        {
            QSet<VPKFilePointer>* set = m_FilesContainingExtensions.value(ext, nullptr);

            if ( !set )
            {
                set = new QSet<VPKFilePointer>();
                m_FilesContainingExtensions.insert(ext, set);
            }

            set->insert(file);
        }
    }

    QSet<VPKFilePointer> VPKFileCollection::filesContainingExtension(const QString &extension) const
    {
        if ( !m_FilesContainingExtensions.contains(extension) )
            return QSet<VPKFilePointer>();

        return *m_FilesContainingExtensions.value(extension);
    }
}
