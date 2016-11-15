#include "vpkfile.h"
#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QScopedArrayPointer>
#include <QByteArray>
#include "vpkarchivemd5item.h"

namespace FileFormats
{
    namespace
    {
        static const char* VPK_DIR_SUFFIX = "_dir";

        inline void setErrorString(QString* errorString, const QString& msg)
        {
            if ( errorString )
                *errorString = msg;
        }

        // We assume we'll always be null-terminated.
        inline QString getString(char*& buffer)
        {
            QString str(buffer);
            buffer += str.length() + 1;
            return str;
        }
    }

    VPKFile::VPKFile(const QString &filename)
        : m_File(filename),
          m_iCurrentArchive(-1)
    {

    }

    VPKFile::~VPKFile()
    {
        close();
    }

    bool VPKFile::open()
    {
        return m_File.open(QIODevice::ReadOnly);
    }

    void VPKFile::close()
    {
        if ( !m_File.isOpen() )
            return;

        m_File.close();
    }

    bool VPKFile::isOpen() const
    {
        return m_File.isOpen();
    }

    bool VPKFile::readIndex(QString *errorHint)
    {
        if ( !isOpen() )
        {
            setErrorString(errorHint, "File is not open.");
            return false;
        }

        {
            QDataStream stream(&m_File);
            stream.setByteOrder(QDataStream::LittleEndian);

            if ( !m_Header.populate(stream, errorHint) ||
                 !validateHeader(errorHint))
                return false;

            m_SiblingArchives = findSiblingArchives();

            if ( !createIndex(stream, errorHint) )
                return false;
        }

        return true;
    }

    bool VPKFile::readArchiveMD5(QString *errorHint)
    {
        if ( !isOpen() )
        {
            setErrorString(errorHint, "File is not open.");
            return false;
        }

        if ( !m_Header.signatureValid() )
        {
            setErrorString(errorHint, "File header is not valid.");
            return false;
        }

        if ( m_Header.version() != 2 )
        {
            setErrorString(errorHint, QString("VPK version %1 does not include an archive MD5 section.").arg(m_Header.version()));
            return false;
        }

        m_File.seek(m_Header.archiveMD5SectionAbsOffset());

        {
            QDataStream stream(&m_File);
            stream.setByteOrder(QDataStream::LittleEndian);

            if ( !readMD5s(stream, errorHint) )
                return false;
        }

        return true;
    }

    bool VPKFile::readOtherMD5(QString *errorHint)
    {
        if ( !isOpen() )
        {
            setErrorString(errorHint, "File is not open.");
            return false;
        }

        if ( !m_Header.signatureValid() )
        {
            setErrorString(errorHint, "File header is not valid.");
            return false;
        }

        if ( m_Header.version() != 2 )
        {
            setErrorString(errorHint, QString("VPK version %1 does not include an \"Other MD5\" section.").arg(m_Header.version()));
            return false;
        }

        m_File.seek(m_Header.otherMD5SectionAbsOffset());

        {
            QDataStream stream(&m_File);
            stream.setByteOrder(QDataStream::LittleEndian);

            if ( !m_OtherMD5s.populate(stream, errorHint) )
                return false;
        }

        return true;
    }

    bool VPKFile::createIndex(QDataStream& stream, QString *errorHint)
    {
        m_Index.clear();

        if ( m_Header.treeSize() < 1 )
        {
            setErrorString(errorHint,
                           QString("Index tree is empty."));
            return false;
        }

        QScopedArrayPointer<char> buffer(new char[m_Header.treeSize()]);

        char* base = &buffer[0];
        int bytesRead = stream.readRawData(base, m_Header.treeSize());
        if ( static_cast<quint32>(bytesRead) != m_Header.treeSize() )
        {
            setErrorString(errorHint,
                           QString("Expected an index tree of %1 bytes but was only able to read %2 bytes.")
                           .arg(m_Header.treeSize())
                           .arg(bytesRead));

            return false;
        }

        char* currentString = base;

        QString currentExtension;
        QString currentPath;
        QString currentFilename;

        while ( currentString < base + m_Header.treeSize() )
        {
            currentExtension = getString(currentString);
            if ( currentExtension.isEmpty() )
                break;

            while ( currentString < base + m_Header.treeSize() )
            {
                currentPath = getString(currentString);
                if ( currentPath.isEmpty() )
                    break;

                while ( currentString < base + m_Header.treeSize() )
                {
                    currentFilename = getString(currentString);
                    if ( currentFilename.isEmpty() )
                        break;

                    if ( currentString + VPKIndexTreeItem::staticSize() > base + m_Header.treeSize() )
                    {
                        setErrorString(errorHint,
                                       QString("Expected index entry of %1 bytes but was only able to read %2 bytes.")
                                       .arg(VPKIndexTreeItem::staticSize())
                                       .arg(reinterpret_cast<quint64>(base) + m_Header.treeSize()
                                            - reinterpret_cast<quint64>(currentString)));

                        return false;
                    }

                    QByteArray data;
                    data.append(currentString, VPKIndexTreeItem::staticSize());
                    currentString += VPKIndexTreeItem::staticSize();

                    QDataStream dataStream(&data, QIODevice::ReadOnly);
                    dataStream.setByteOrder(QDataStream::LittleEndian);

                    if ( !createRecord(dataStream, currentPath, currentFilename,
                                       currentExtension, errorHint) )
                        return false;
                }
            }
        }

        return true;
    }

    bool VPKFile::createRecord(QDataStream &stream, const QString &path, const QString &filename,
                               const QString &extension, QString *errorHint)
    {
        VPKIndexTreeRecordPointer record =
                VPKIndexTreeRecordPointer::create(path, filename, extension);

        if ( !record->item()->populate(stream, errorHint) )
            return false;

        m_Index.addRecord(record);
        return true;
    }

    const VPKHeader& VPKFile::header() const
    {
        return m_Header;
    }

    const VPKIndex& VPKFile::index() const
    {
        return m_Index;
    }

    QStringList VPKFile::findSiblingArchives() const
    {
        QFileInfo fileInfo(m_File.fileName());
        QString baseName = fileInfo.baseName();
        if ( !baseName.endsWith(VPK_DIR_SUFFIX) )
        {
            return QStringList();
        }

        baseName = baseName.left(baseName.length() - static_cast<int>(strlen(VPK_DIR_SUFFIX)));

        QDir dir = fileInfo.dir();
        QStringList list = dir.entryList(QStringList() << (baseName + "*"), QDir::Files, QDir::Name);

        QRegularExpression regex(baseName + "_[0-9]+\\.vpk");

        QStringList outList;
        QString prefix = fileInfo.canonicalPath() + "/";

        foreach ( const QString& str, list )
        {
            if ( regex.match(str).hasMatch() )
            {
                outList << prefix + str;
            }
        }

        return outList;
    }

    QStringList VPKFile::siblingArchives() const
    {
        return m_SiblingArchives;
    }

    bool VPKFile::openArchive(int index)
    {
        if ( index < 0 || index >= m_SiblingArchives.count() || m_Archive.isOpen() )
            return false;

        m_Archive.setFileName(m_SiblingArchives.at(index));

        if ( !m_Archive.open(QIODevice::ReadOnly) )
            return false;

        m_iCurrentArchive = index;
        return true;
    }

    bool VPKFile::isArchiveOpen() const
    {
        return m_Archive.isOpen();
    }

    void VPKFile::closeArchive()
    {
        if ( isArchiveOpen() )
            return;

        m_Archive.close();
        m_iCurrentArchive = -1;
    }

    QByteArray VPKFile::readFromCurrentArchive(const VPKIndexTreeItem* item)
    {
        if ( !isArchiveOpen() || item->archiveIndex() != m_iCurrentArchive )
            return QByteArray();

        if ( !m_Archive.seek(item->entryOffset()) )
            return QByteArray();

        return m_Archive.read(item->entryLength());
    }

    int VPKFile::currentArchiveIndex() const
    {
        return m_iCurrentArchive;
    }

    bool VPKFile::validateHeader(QString *errorHint) const
    {
        if ( m_Header.archiveMD5SectionSize() % VPKArchiveMD5Item::staticSize() != 0 )
        {
            setErrorString(errorHint,
                           QString("Expected archive MD5 section (%1 bytes) to be a multiple of %2 bytes.")
                           .arg(m_Header.archiveMD5SectionSize())
                           .arg(VPKArchiveMD5Item::staticSize()));
            return false;
        }

        return true;
    }

    const VPKArchiveMD5Collection& VPKFile::archiveMD5Collection() const
    {
        return m_ArchiveMD5Collection;
    }

    const VPKOtherMD5Item& VPKFile::otherMD5s() const
    {
        return m_OtherMD5s;
    }

    QByteArray VPKFile::treeData()
    {
        if ( !isOpen() || !m_Header.signatureValid() )
            return QByteArray();

        m_File.seek(m_Header.treeAbsOffset());
        return m_File.read(m_Header.treeSize());
    }

    QByteArray VPKFile::archiveMD5Data()
    {
        if ( !isOpen() || !m_Header.signatureValid() )
            return QByteArray();

        m_File.seek(m_Header.archiveMD5SectionAbsOffset());
        return m_File.read(m_Header.archiveMD5SectionSize());
    }

    bool VPKFile::readMD5s(QDataStream &stream, QString *errorHint)
    {
        m_ArchiveMD5Collection.clear();

        for ( quint32 bytesRead = 0; bytesRead < m_Header.archiveMD5SectionSize(); bytesRead += VPKArchiveMD5Item::staticSize() )
        {
            VPKArchiveMD5ItemPointer md5 = VPKArchiveMD5ItemPointer::create();
            if ( !md5->populate(stream, errorHint) )
            {
                m_ArchiveMD5Collection.clear();
                return false;
            }

            m_ArchiveMD5Collection.addItem(md5);
        }

        return true;
    }
}
