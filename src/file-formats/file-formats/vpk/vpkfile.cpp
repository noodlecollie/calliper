#include "vpkfile.h"
#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QScopedArrayPointer>
#include <QByteArray>
#include "vpkarchivemd5item.h"
#include "calliperutil/general/paths.h"
#include "vpkindextreeiterator.h"

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
    }

    VPKFile::VPKFile(const QString &filename)
        : m_File(filename),
          m_iCurrentArchive(-1)
    {

    }

    VPKFile::VPKFile()
        : m_File(),
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

        QByteArray buffer(m_Header.treeSize(), 0);
        int bytesRead = stream.readRawData(buffer.data(), m_Header.treeSize());
        if ( static_cast<quint32>(bytesRead) != m_Header.treeSize() )
        {
            setErrorString(errorHint,
                           QString("Expected an index tree of %1 bytes but was only able to read %2 bytes.")
                           .arg(m_Header.treeSize())
                           .arg(bytesRead));

            return false;
        }

        VPKIndexTreeIterator it(&buffer);
        for ( ; it.isValid(); it.advance() )
        {
            createRecord(it);
        }

        QString itErrorHint = it.errorHint();
        if ( !itErrorHint.isNull() )
        {
            setErrorString(errorHint, itErrorHint);
            return false;
        }

        return true;
    }

    void VPKFile::createRecord(const VPKIndexTreeIterator &it)
    {
        VPKIndexTreeRecordPointer record =
                VPKIndexTreeRecordPointer::create(it);

        m_Index.addRecord(record);
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

    int VPKFile::siblingArchiveCount() const
    {
        return m_SiblingArchives.count();
    }

    bool VPKFile::openArchive(int index)
    {
        if ( index < 0 || index >= m_SiblingArchives.count() )
            return false;

        if ( isArchiveOpen() )
            closeArchive();

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
        if ( !isArchiveOpen() )
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

    QString VPKFile::fileName() const
    {
        return m_File.fileName();
    }

    void VPKFile::setFileName(const QString &filename)
    {
        if ( filename == m_File.fileName() )
            return;

        clear();
        m_File.setFileName(filename);
    }

    void VPKFile::clear()
    {
        closeArchive();
        close();

        m_File.setFileName(QString());
        m_SiblingArchives.clear();
        m_Header.clear();
        m_Index.clear();
        m_ArchiveMD5Collection.clear();
        m_OtherMD5s.clear();
    }
}
