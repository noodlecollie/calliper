#ifndef VPKFILECOLLECTION_H
#define VPKFILECOLLECTION_H

#include "file-formats_global.h"
#include <QSharedPointer>
#include "vpkfile.h"
#include <QList>
#include <QHash>
#include <QSet>

namespace FileFormats
{
    typedef QSharedPointer<VPKFile> VPKFilePointer;

    class VPKFileCollection
    {
    public:
        VPKFileCollection();
        VPKFileCollection(const QString& path);
        ~VPKFileCollection();

        // Assumes file index is loaded before being added.
        void addFile(const VPKFilePointer& file);

        QSet<VPKFilePointer> filesContainingExtension(const QString& extension) const;

    private:
        typedef QHash<QString, QSet<VPKFilePointer>*> ExtensionTable;

        void processAddFile(const VPKFilePointer& file);

        QList<VPKFilePointer> m_Files;
        ExtensionTable m_FilesContainingExtensions;
    };
}

#endif // VPKFILECOLLECTION_H
