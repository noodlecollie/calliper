#include "vpkothermd5item.h"

namespace FileFormats
{
    struct VPKOtherMD5Item::Data
    {
        char treeChecksum[16];
        char archiveMD5SectionChecksum[16];
        char unknown1[16];
    };

    VPKOtherMD5Item::VPKOtherMD5Item()
        : StreamDataContainer(),
          m_pData(new VPKOtherMD5Item::Data())
    {
        clear();
    }

    VPKOtherMD5Item::~VPKOtherMD5Item()
    {
        delete m_pData;
    }

    bool VPKOtherMD5Item::populate(QDataStream &stream, QString *errorHint)
    {
        VPKOtherMD5Item::Data& d = *m_pData;

        beginRead(stream);

        stream.readRawData(d.treeChecksum, 16 * sizeof(char));
        stream.readRawData(d.archiveMD5SectionChecksum, 16 * sizeof(char));
        stream.readRawData(d.unknown1, 16 * sizeof(char));

        if ( !endRead(stream, errorHint) )
            return false;

        return true;
    }

    void VPKOtherMD5Item::clear()
    {
        memset(m_pData, 0, sizeof(VPKOtherMD5Item::Data));
    }

    QString VPKOtherMD5Item::containerName() const
    {
        return "VPK Other MD5 Item";
    }

    quint32 VPKOtherMD5Item::staticSize()
    {
        return (16 + 16 + 16) * sizeof(char);
    }

    QByteArray VPKOtherMD5Item::treeChecksum() const
    {
        return QByteArray(m_pData->treeChecksum, 16 * sizeof(char));
    }

    QByteArray VPKOtherMD5Item::archiveMD5SectionChecksum() const
    {
        return QByteArray(m_pData->archiveMD5SectionChecksum, 16 * sizeof(char));
    }

    QByteArray VPKOtherMD5Item::unknownProperty1() const
    {
        return QByteArray(m_pData->unknown1, 16 * sizeof(char));
    }
}
