#include "vpkfile.h"
#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QScopedArrayPointer>
#include <QByteArray>

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

    bool VPKFile::read(QString *errorHint)
    {
        if ( !m_File.open(QIODevice::ReadOnly) )
        {
            setErrorString(errorHint, "Could not open " + m_File.fileName());
            return false;
        }

        {
            QDataStream stream(&m_File);
            stream.setByteOrder(QDataStream::LittleEndian);

            if ( !m_Header.populate(stream, errorHint) )
                return false;

            m_SiblingArchives = findSiblingArchives();

            if ( !createIndex(stream, errorHint) )
                return false;
        }

        m_File.close();
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

        foreach ( const QString& str, list )
        {
            if ( regex.match(str).hasMatch() )
            {
                outList << str;
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
        // TODO: We don't currently check for preload bytes.
        // How should these be handled? Should they be kept in the index?
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
}
