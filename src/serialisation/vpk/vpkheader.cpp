#include "vpkheader.h"
#include <QtDebug>

namespace NS_SERIALISATION
{
    namespace
    {
        inline void setErrorString(QString* errorString, const QString& msg)
        {
            if ( errorString )
                *errorString = msg;
        }
    }

    class VPKHeader::Data
    {
        quint32 signature;
        quint32 version;

        quint32 treeSize;
        quint32 fileDataSectionSize;
        quint32 archiveMD5SectionSize;
        quint32 otherMD5SectionSize;
        quint32 signatureSectionSize;
    };

    VPKHeader::VPKHeader()
        : m_pData(new VPKHeader::Data())
    {
        memset(m_pData.data(), 0, sizeof(VPKHeader::Data));
    }

    bool VPKHeader::populate(QDataStream &stream, QString *errorHint)
    {
        VPKHeader::Data& d = *m_pData;

        if ( stream.byteOrder() != QDataStream::LittleEndian )
        {
            qWarning() << "Byte order of VPK input stream is not little endian, reading will probably fail!";
        }

        stream >> d.signature;
        if ( signature != 0x55aa1234 )
        {
            setErrorString(errorHint, "Incorrect VPK signature.");
            return false;
        }

        stream >> d.version;
        if ( d.version == 1 )
            return true;

        if ( d.version != 2 )
        {
            setErrorString(errorHint, QString("Unexpected VPK version '%1'.").arg(d.version));
            return false;
        }

        stream
                >> d.treeSize
                >> d.fileDataSectionSize
                >> d.archiveMD5SectionSize
                >> d.otherMD5SectionSize
                >> d.signatureSectionSize;

        return true;
    }

    quint32 VPKHeader::signature() const
    {
        return m_pData->signature;
    }

    quint32 VPKHeader::version() const
    {
        return m_pData->version;
    }

    quint32 VPKHeader::treeSize() const
    {
        return m_pData->treeSize;
    }

    quint32 VPKHeader::fileDataSectionSize() const
    {
        return m_pData->fileDataSectionSize;
    }

    quint32 VPKHeader::archiveMD5SectionSize() const
    {
        return m_pData->archiveMD5SectionSize;
    }

    quint32 VPKHeader::otherMD5SectionSize() const
    {
        return m_pData->otherMD5SectionSize;
    }

    quint32 VPKHeader::signatureSectionSize() const
    {
        return m_pData->signatureSectionSize;
    }
}
