#ifndef VPKINDEXTREEITEM_H
#define VPKINDEXTREEITEM_H

#include "file-formats_global.h"
#include <QDataStream>

namespace FileFormats
{
    class VPKIndexTreeItem
    {
    public:
        VPKIndexTreeItem();
        ~VPKIndexTreeItem();

        bool populate(QDataStream& stream, QString* errorHint = nullptr);

        quint32 crc() const;
        quint16 preloadBytes() const;
        quint16 archiveIndex() const;
        quint32 entryOffset() const;
        quint32 entryLength() const;

        static quint32 staticSize();

    private:
        struct Data;
        Data* m_pData;
    };
}

#endif // VPKINDEXTREEITEM_H
