#ifndef VPKFILE_H
#define VPKFILE_H

#include "file-formats_global.h"
#include "vpkindex.h"
#include <QFile>
#include "vpkheader.h"
#include <QDataStream>
#include <QList>
#include "vpkarchivemd5collection.h"
#include "vpkothermd5collection.h"

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
        const VPKArchiveMD5Collection& archiveMD5Collection() const;
        const VPKOtherMD5Collection& otherMD5Collection() const;

        bool openArchive(int index);
        bool isArchiveOpen() const;
        void closeArchive();
        QByteArray readFromCurrentArchive(const VPKIndexTreeItem* item);
        int currentArchiveIndex() const;

    private:
        typedef QSharedPointer<VPKArchiveMD5Item> VPKArchiveMD5ItemPointer;
        typedef QSharedPointer<VPKOtherMD5Item> VPKOtherMD5ItemPointer;

        bool createIndex(QDataStream& stream, QString* errorHint);
        bool createRecord(QDataStream& stream, const QString& path, const QString& filename,
                          const QString& extension, QString* errorHint);
        QStringList findSiblingArchives() const;
        bool validateHeader(QString* errorHint) const;

        QFile m_File;
        VPKHeader m_Header;
        VPKIndex m_Index;
        VPKArchiveMD5Collection m_ArchiveMD5Collection;
        VPKOtherMD5Collection m_OtherMD5Collection;

        QStringList m_SiblingArchives;
        QFile m_Archive;
        int m_iCurrentArchive;
    };
}

#endif // VPKFILE_H
