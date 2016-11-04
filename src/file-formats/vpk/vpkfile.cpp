#include "vpkfile.h"
#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>

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

        inline void getNullTerminatedString(QDataStream& stream, QString& strOut)
        {
            strOut.clear();
            char data[9];
            memset(data, 0, 9);

            stream.startTransaction();

            do
            {
                stream.readRawData(data, 8);
                strOut += data;
            }
            while ( strlen(data) == 8 );

            stream.rollbackTransaction();
            stream.skipRawData(strOut.length() + 1);
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
        while ( true )
        {
            getNullTerminatedString(stream, m_strCurrentExtension);
            if ( m_strCurrentExtension.isEmpty() )
                break;

            while ( true )
            {
                getNullTerminatedString(stream, m_strCurrentPath);
                if ( m_strCurrentPath.isEmpty() )
                    break;

                while ( true )
                {
                    getNullTerminatedString(stream, m_strCurrentFilename);
                    if ( m_strCurrentFilename.isEmpty() )
                        break;

                    if ( !createRecord(stream, errorHint) )
                        return false;
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

        baseName = baseName.left(baseName.length() - strlen(VPK_DIR_SUFFIX));

        QDir dir = fileInfo.dir();
        QStringList list = dir.entryList(QStringList() << (baseName + "*"), QDir::Files, QDir::Name);

        QRegularExpression regex(baseName + "_[0-9]+\.vpk");

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
