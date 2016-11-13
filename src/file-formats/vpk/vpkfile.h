#ifndef VPKFILE_H
#define VPKFILE_H

#include "file-formats_global.h"
#include "vpkindex.h"
#include <QFile>
#include "vpkheader.h"
#include <QDataStream>
#include <QList>
#include "vpkarchivemd5item.h"
#include "vpkothermd5item.h"

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKFile
    {
    public:
        VPKFile(const QString& filename);
        ~VPKFile();

        QStringList siblingArchives() const;

        bool open();
        void close();
        bool isOpen() const;

        bool readIndex(QString* errorHint = nullptr);
        bool readArchiveMD5(QString* errorHint = nullptr);
        bool readOtherMD5(QString* errorHint = nullptr);

        const VPKHeader& header() const;
        const VPKIndex& index() const;

        bool openArchive(int index);
        bool isArchiveOpen() const;
        void closeArchive();
        QByteArray readFromCurrentArchive(const VPKIndexTreeItem* item);
        int currentArchiveIndex() const;

        int archiveMD5Count() const;
        QSharedPointer<const VPKArchiveMD5Item> archiveMD5(int index) const;

        int otherMD5Count() const;
        QSharedPointer<const VPKOtherMD5Item> otherMD5(int index) const;

    private:
        typedef QSharedPointer<VPKArchiveMD5Item> VPKArchiveMD5ItemPointer;
        typedef QSharedPointer<VPKOtherMD5Item> VPKOtherMD5ItemPointer;

        bool createIndex(QDataStream& stream, QString* errorHint);
        bool createRecord(QDataStream& stream, const QString& path, const QString& filename,
                          const QString& extension, QString* errorHint);
        QStringList findSiblingArchives() const;
        bool validateHeader(QString* errorHint) const;
        bool readArchiveMD5s(QDataStream& stream, QString* errorHint);
        bool readOtherMD5s(QDataStream& stream, QString* errorHint);

        QFile m_File;
        VPKHeader m_Header;
        VPKIndex m_Index;

        QStringList m_SiblingArchives;
        QFile m_Archive;
        int m_iCurrentArchive;

        QList<VPKArchiveMD5ItemPointer> m_ArchiveMD5s;
        QList<VPKOtherMD5ItemPointer> m_OtherMD5s;
    };
}

#endif // VPKFILE_H
