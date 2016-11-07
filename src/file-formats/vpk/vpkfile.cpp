#include "vpkfile.h"
#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QScopedArrayPointer>

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
        : m_File(filename)
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

            createIndex(stream, errorHint);
        }

        m_File.close();
        return true;
    }

    bool VPKFile::createIndex(QDataStream& stream, QString *errorHint)
    {
        if ( !m_Header.populate(stream, errorHint) )
            return false;

        m_strCurrentExtension = QString();
        m_strCurrentPath = QString();
        m_strCurrentFilename = QString();
        m_Index.clear();

        return populateIndex(stream, errorHint);
    }

    bool VPKFile::populateIndex(QDataStream &stream, QString* errorHint)
    {
        if ( m_Header.treeSize() < 1 )
        {
            setErrorString(errorHint,
                           QString("Index tree is empty."));
            return false;
        }

        QScopedArrayPointer<char> buffer(new char[m_Header.treeSize()]);

        char* base = &buffer[0];
        int bytesRead = stream.readRawData(base, m_Header.treeSize());
        if ( bytesRead != m_Header.treeSize() )
        {
            setErrorString(errorHint,
                           QString("Expected an index tree of %1 bytes but was only able to read %2 bytes.")
                           .arg(m_Header.treeSize())
                           .arg(bytesRead));

            return false;
        }

        char* currentString = base;

        while ( currentString < base + m_Header.treeSize() )
        {
            m_strCurrentExtension = getString(currentString);
            if ( m_strCurrentExtension.isEmpty() )
                break;

            while ( currentString < base + m_Header.treeSize() )
            {
                m_strCurrentPath = getString(currentString);
                if ( m_strCurrentPath.isEmpty() )
                    break;

                while ( currentString < base + m_Header.treeSize() )
                {
                    m_strCurrentFilename = getString(currentString);
                    if ( m_strCurrentFilename.isEmpty() )
                        break;

                    if ( !createRecord(stream, errorHint) )
                        return false;

                    currentString += VPKIndexTreeItem::staticSize();
                }
            }
        }

        return true;
    }

    bool VPKFile::createRecord(QDataStream &stream, QString* errorHint)
    {
        VPKIndexTreeRecordPointer record =
                VPKIndexTreeRecordPointer::create(m_strCurrentPath, m_strCurrentFilename, m_strCurrentExtension);

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

    QStringList VPKFile::siblingArchives() const
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
}
