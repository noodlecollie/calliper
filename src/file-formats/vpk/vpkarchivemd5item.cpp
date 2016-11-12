#include "vpkarchivemd5item.h"
#include <QDataStream>

namespace FileFormats
{
    struct VPKArchiveMD5Item::Data
    {
        quint32 archiveIndex;
        quint32 startingOffset;
        quint32 count;
        char md5[16];
    };

    VPKArchiveMD5Item::VPKArchiveMD5Item()
        : m_pData(new VPKArchiveMD5Item::Data())
    {
        memset(m_pData, 0, sizeof(VPKArchiveMD5Item::Data));
    }

    VPKArchiveMD5Item::~VPKArchiveMD5Item()
    {
        delete m_pData;
    }

    bool VPKArchiveMD5Item::populate(QDataStream &stream, QString *errorHint)
    {
        VPKArchiveMD5Item::Data& d = *m_pData;

        beginRead(stream);

        stream >> d.archiveIndex;
        stream >> d.startingOffset;
        stream >> d.count;
        stream.readRawData(d.md5, 16 * sizeof(char));

        if ( !endRead(stream, errorHint) )
            return false;

        return true;
    }

    quint32 VPKArchiveMD5Item::size()
    {
        return (3 * sizeof(quint32)) + (16 * sizeof(char));
    }

    quint32 VPKArchiveMD5Item::archiveIndex() const
    {
        return m_pData->archiveIndex;
    }

    quint32 VPKArchiveMD5Item::startingOffset() const
    {
        return m_pData->startingOffset;
    }

    quint32 VPKArchiveMD5Item::count() const
    {
        return m_pData->count;
    }

    QByteArray VPKArchiveMD5Item::md5() const
    {
        return QByteArray(m_pData->md5, 16 * sizeof(char));
    }

    QString VPKArchiveMD5Item::containerName() const
    {
        return "VPK Archive MD5 Item";
    }
}
