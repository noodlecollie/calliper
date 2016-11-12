#ifndef VPKFILE_H
#define VPKFILE_H

#include "file-formats_global.h"
#include "vpkindex.h"
#include <QFile>
#include "vpkheader.h"
#include "vpkheader.h"
#include <QDataStream>

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKFile
    {
    public:
        VPKFile(const QString& filename);

        QStringList siblingArchives() const;

        bool read(QString* errorHint = nullptr);

        const VPKHeader& header() const;
        const VPKIndex& index() const;

        bool openArchive(int index);
        bool isArchiveOpen() const;
        void closeArchive();
        QByteArray readFromCurrentArchive(const VPKIndexTreeItem* item);
        int currentArchiveIndex() const;

    private:
        bool createIndex(QDataStream& stream, QString* errorHint);
        bool createRecord(QDataStream& stream, const QString& path, const QString& filename,
                          const QString& extension, QString* errorHint);
        QStringList findSiblingArchives() const;

        QFile m_File;
        VPKHeader m_Header;
        VPKIndex m_Index;

        QStringList m_SiblingArchives;
        QFile m_Archive;
        int m_iCurrentArchive;
    };
}

#endif // VPKFILE_H
