#include "vpkparser.h"
#include <QDataStream>

namespace FileFormats
{
    namespace
    {
        inline void setErrorString(QString* errorString, const QString& msg)
        {
            if ( errorString )
                *errorString = msg;
        }
    }

    VPKParser::VPKParser(const QString &filename)
        : m_File(filename)
    {

    }

    bool VPKParser::open()
    {
        return m_File.open(QIODevice::ReadOnly);
    }

    void VPKParser::close()
    {
        if ( !m_File.isOpen() )
            return;

        m_File.close();
    }

    bool VPKParser::isOpen() const
    {
        return m_File.isOpen();
    }

    bool VPKParser::createIndex(VPKIndex &index, QString *errorHint)
    {
        if ( !isOpen() )
        {
            setErrorString(errorHint, "VPK file is not open yet.");
            return false;
        }

        QDataStream stream(&m_File);
        stream.setByteOrder(QDataStream::LittleEndian);

        if ( !m_Header.populate(stream, errorHint) )
            return false;

        m_strCurrentExtension = QString();
        m_strCurrentPath = QString();
        m_strCurrentFilename = QString();

        return populateIndex(index, stream, errorHint);
    }

    bool VPKParser::populateIndex(VPKIndex &index, QDataStream &stream, QString* errorHint)
    {
        while ( true )
        {
            stream >> m_strCurrentExtension;
            if ( m_strCurrentExtension.isEmpty() )
                break;

            while ( true )
            {
                stream >> m_strCurrentPath;
                if ( m_strCurrentPath.isEmpty() )
                    break;

                while ( true )
                {
                    stream >> m_strCurrentFilename;
                    if ( m_strCurrentFilename.isEmpty() )
                        break;

                    if ( !createRecord(index, stream, errorHint) )
                        return false;
                }
            }
        }

        return true;
    }

    bool VPKParser::createRecord(VPKIndex &index, QDataStream &stream, QString* errorHint)
    {
        VPKIndexTreeRecordPointer record =
                VPKIndexTreeRecordPointer::create(m_strCurrentPath, m_strCurrentFilename, m_strCurrentExtension);

        if ( !record->item()->populate(stream, errorHint) )
            return false;

        index.addRecord(record);
        return true;
    }
}
