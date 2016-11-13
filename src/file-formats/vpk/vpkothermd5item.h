#ifndef VPKOTHERMD5ITEM_H
#define VPKOTHERMD5ITEM_H

#include "file-formats_global.h"
#include "streamdatacontainer.h"

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKOtherMD5Item : public StreamDataContainer
    {
    public:
        VPKOtherMD5Item();
        ~VPKOtherMD5Item();

        virtual bool populate(QDataStream& stream, QString* errorHint = nullptr) override;

    protected:
        virtual QString containerName() const override;

    private:
        struct Data;
        Data* m_pData;
    };
}

#endif // VPKOTHERMD5ITEM_H
