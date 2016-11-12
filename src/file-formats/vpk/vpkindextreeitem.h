#ifndef VPKINDEXTREEITEM_H
#define VPKINDEXTREEITEM_H

#include "file-formats_global.h"
#include "streamdatacontainer.h"
#include <QDataStream>

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKIndexTreeItem : public StreamDataContainer
    {
    public:
        VPKIndexTreeItem();
        ~VPKIndexTreeItem();

        virtual bool populate(QDataStream& stream, QString* errorHint = nullptr) override;

        quint32 crc() const;
        quint16 preloadBytes() const;
        quint16 archiveIndex() const;
        quint32 entryOffset() const;
        quint32 entryLength() const;

        static quint32 staticSize();

    protected:
        virtual QString containerName() const override;

    private:
        struct Data;
        Data* m_pData;
    };
}

#endif // VPKINDEXTREEITEM_H
