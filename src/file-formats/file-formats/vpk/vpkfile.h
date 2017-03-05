#ifndef VPKFILE_H
#define VPKFILE_H

#include "file-formats_global.h"
#include "vpkindex.h"
#include <QFile>
#include "vpkheader.h"
#include <QDataStream>
#include <QList>
#include "vpkarchivemd5collection.h"
#include "vpkothermd5item.h"
#include "vpkindextreeiterator.h"

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKFile
    {
    public:
        VPKFile();
        VPKFile(const QString& filename);
        ~VPKFile();

        QStringList siblingArchives() const;
        int siblingArchiveCount() const;

        bool open();
        void close();
        bool isOpen() const;
        QString fileName() const;
        void setFileName(const QString& filename);
        void clear();

        bool readIndex(QString* errorHint = Q_NULLPTR);
        bool readArchiveMD5(QString* errorHint = Q_NULLPTR);
        bool readOtherMD5(QString* errorHint = Q_NULLPTR);

        const VPKHeader& header() const;
        const VPKIndex& index() const;
        const VPKArchiveMD5Collection& archiveMD5Collection() const;
        const VPKOtherMD5Item& otherMD5s() const;

        bool openArchive(int index);
        bool isArchiveOpen() const;
        void closeArchive();
        QByteArray readFromCurrentArchive(const VPKIndexTreeItem* item);
        int currentArchiveIndex() const;

        // Read from file on demand.
        QByteArray treeData();
        QByteArray archiveMD5Data();

    private:
        typedef QSharedPointer<VPKArchiveMD5Item> VPKArchiveMD5ItemPointer;
        typedef QSharedPointer<VPKOtherMD5Item> VPKOtherMD5ItemPointer;

        bool createIndex(QDataStream& stream, QString* errorHint);
        void createRecord(const VPKIndexTreeIterator& it);
        QStringList findSiblingArchives() const;
        bool validateHeader(QString* errorHint) const;
        bool readMD5s(QDataStream& stream, QString* errorHint);

        QFile m_File;
        VPKHeader m_Header;
        VPKIndex m_Index;
        VPKArchiveMD5Collection m_ArchiveMD5Collection;
        VPKOtherMD5Item m_OtherMD5s;

        QStringList m_SiblingArchives;
        QFile m_Archive;
        int m_iCurrentArchive;
    };
}

#endif // VPKFILE_H
