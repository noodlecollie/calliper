#include "vpkindextreeitem.h"

namespace FileFormats
{
    namespace
    {
        inline void setErrorString(QString* errorString, const QString& msg)
        {
            if ( errorString )
                *errorString = msg;
        }

        template<typename T>
        inline QString toHex(const T& val, bool leading0x = false)
        {
            return QString(leading0x ? "0x%1" : "%1").arg(val, sizeof(T)*2, 16);
        }
    }

    struct VPKIndexTreeItem::Data
    {
        quint32 crc;
        quint16 preloadBytes;
        quint16 archiveIndex;
        quint32 entryOffset;
        quint32 entryLength;
        quint16 terminator;     // Should always be 0xffff.
    };

    VPKIndexTreeItem::VPKIndexTreeItem()
        : m_pData(new VPKIndexTreeItem::Data())
    {
        memset(m_pData, 0, sizeof(VPKIndexTreeItem::Data));
    }

    VPKIndexTreeItem::~VPKIndexTreeItem()
    {
        delete m_pData;
    }

    quint32 VPKIndexTreeItem::staticSize()
    {
        return (3 * sizeof(quint32)) + (3 * sizeof(quint16));
    }

    bool VPKIndexTreeItem::populate(QDataStream &stream, QString *errorHint)
    {
        VPKIndexTreeItem::Data& d = *m_pData;

        stream >> d.crc
                >> d.preloadBytes
                >> d.archiveIndex
                >> d.entryOffset
                >> d.entryLength
                >> d.terminator;

        // Double check this just in case.
        if ( d.terminator != 0xffff )
        {
            setErrorString(errorHint, QString("Unexpected terminator value %1").arg(toHex<quint16>(d.terminator, true)));
            return false;
        }

        return true;
    }
}
