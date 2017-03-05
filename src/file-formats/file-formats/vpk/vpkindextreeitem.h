#ifndef VPKINDEXTREEITEM_H
#define VPKINDEXTREEITEM_H

#include "file-formats_global.h"
#include "file-formats/common/streamdatacontainer.h"
#include <QDataStream>

namespace FileFormats
{
    // TODO: Refactor this so that it doesn't use memcpy
    class FILEFORMATSSHARED_EXPORT VPKIndexTreeItem : public StreamDataContainer
    {
    public:
        VPKIndexTreeItem();
        ~VPKIndexTreeItem();

        VPKIndexTreeItem(const VPKIndexTreeItem& other);
        VPKIndexTreeItem& operator =(const VPKIndexTreeItem& other);

        virtual bool populate(QDataStream& stream, QString* errorHint = Q_NULLPTR) override;
        void clear();

        quint32 crc() const;
        quint16 preloadBytes() const;
        quint16 archiveIndex() const;
        quint32 entryOffset() const;
        quint32 entryLength() const;

        QByteArray preloadData() const;

        // Size of entries excluding any preload data,
        // but including the terminator.
        static quint32 staticSize();

        // Size of all data, including preload bytes.
        quint32 totalSize() const;

        // Size of the file this item represents.
        // This is preloadBytes + entryLength.
        quint32 fileSize() const;

    protected:
        virtual QString containerName() const override;

    private:
        struct Data;
        Data* m_pData;
        QByteArray m_PreloadData;
    };
}

#endif // VPKINDEXTREEITEM_H
