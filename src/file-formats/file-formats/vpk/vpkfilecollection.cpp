#include "vpkfilecollection.h"
#include <QDir>
#include <QtDebug>

namespace FileFormats
{
    VPKFileCollection::VPKFileCollection()
    {

    }

    VPKFileCollection::VPKFileCollection(const QString &path)
    {
        addFilesFromDirectory(path);
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

    void VPKFileCollection::addFilesFromDirectory(const QString &path)
    {
        QDir dir(path);

        QFileInfoList vpkFiles = dir.entryInfoList(
                    QStringList() << "*_dir.vpk",
                    QDir::Files);

        foreach ( const QFileInfo& file, vpkFiles )
        {
            VPKFilePointer filePointer = VPKFilePointer::create(file.canonicalFilePath());

            if ( !filePointer->open() )
                continue;

            if ( filePointer->readIndex() )
            {
                addFile(filePointer);
            }

            filePointer->close();
        }
    }

    void VPKFileCollection::clear()
    {
        m_Files.clear();
        m_FilesContainingExtensions.clear();
    }
}
