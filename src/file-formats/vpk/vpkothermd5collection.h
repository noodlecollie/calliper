#ifndef VPKOTHERMD5COLLECTION_H
#define VPKOTHERMD5COLLECTION_H

#include "file-formats_global.h"
#include "collection/simpleitemcollection.h"
#include "vpkothermd5item.h"

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKOtherMD5Collection : public SimpleItemCollection<VPKOtherMD5Item>
    {
    public:
        VPKOtherMD5Collection() : SimpleItemCollection<VPKOtherMD5Item>()
        {
        }
    };
}

#endif // VPKOTHERMD5COLLECTION_H
