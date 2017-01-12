#ifndef VPKARCHIVEMD5ITEM_H
#define VPKARCHIVEMD5ITEM_H

#include "file-formats_global.h"
#include "common/streamdatacontainer.h"

namespace FileFormats
{
    // TODO: Refactor this so that it doesn't use memcpy
    class FILEFORMATSSHARED_EXPORT VPKArchiveMD5Item : public StreamDataContainer
    {
    public:
        VPKArchiveMD5Item();
        ~VPKArchiveMD5Item();

        virtual bool populate(QDataStream& stream, QString* errorHint = nullptr) override;

        quint32 archiveIndex() const;
        quint32 startingOffset() const;
        quint32 count() const;
        QByteArray md5() const;

        static quint32 staticSize();

    protected:
        virtual QString containerName() const override;

    private:
        struct Data;
        Data* m_pData;
    };
}

#endif // VPKARCHIVEMD5ITEM_H
