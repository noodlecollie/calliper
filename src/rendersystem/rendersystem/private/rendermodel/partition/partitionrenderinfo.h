#ifndef PARTITIONRENDERINFO_H
#define PARTITIONRENDERINFO_H

#include "rendersystem/private/shaders/common/privateshaderdefs.h"
#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

struct PartitionRenderInfo
{
public:
    PrivateShaderDefs::ShaderId shaderId;
    RenderSystem::PublicStoreDefs::MaterialId materialId;

    PartitionRenderInfo();
    PartitionRenderInfo(PrivateShaderDefs::ShaderId shader,
                        RenderSystem::PublicStoreDefs::MaterialId material);
};

#endif // PARTITIONRENDERINFO_H
