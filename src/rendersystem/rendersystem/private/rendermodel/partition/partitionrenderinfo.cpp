#include "partitionrenderinfo.h"

PartitionRenderInfo::PartitionRenderInfo()
    : shaderId(PrivateShaderDefs::UnknownShaderId),
      materialId(RenderSystem::PublicStoreDefs::INVALID_MATERIAL_ID)
{

}

PartitionRenderInfo::PartitionRenderInfo(PrivateShaderDefs::ShaderId shader,
                                         RenderSystem::PublicStoreDefs::MaterialId material)
    : shaderId(shader),
      materialId(material)
{

}
