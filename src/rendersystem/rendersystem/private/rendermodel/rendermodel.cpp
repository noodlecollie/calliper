#include "rendermodel.h"

#include "rendersystem/endpoints/materialstoreendpoint.h"
#include "rendersystem/endpoints/texturestoreendpoint.h"
#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

RenderModel::RenderGroupKey::RenderGroupKey()
    : shaderStyle(RenderSystem::PublicShaderDefs::UnknownShaderStyle),
      mainTextureId(RenderSystem::PublicStoreDefs::INVALID_TEXTURE_ID),
      secondaryTextureId(RenderSystem::PublicStoreDefs::INVALID_TEXTURE_ID)
{
}

RenderModel::RenderGroupKey::RenderGroupKey(RenderSystem::PublicStoreDefs::MaterialId materialId)
    : RenderGroupKey()
{
    using namespace RenderSystem;

    CurrentContextGuard<IMaterialStore> materialStore = MaterialStoreEndpoint::materialStore();
    QSharedPointer<RenderMaterial> material = materialStore->material(materialId).toStrongRef();

    shaderStyle = material->shaderStyle();

    static_assert(false, "Set main and secondary textures - material returns their paths");
}

bool RenderModel::RenderGroupKey::operator <(const RenderModel::RenderGroupKey& other) const
{
    if ( shaderStyle != other.shaderStyle )
    {
        return shaderStyle < other.shaderStyle;
    }

    if ( mainTextureId != other.mainTextureId )
    {
        return mainTextureId < other.mainTextureId;
    }

    return secondaryTextureId < other.secondaryTextureId;
}

bool RenderModel::renderGroupExists(const RenderGroupKey &key) const
{
    return m_RenderGroups.contains(key);
}

bool RenderModel::renderGroupExists(RenderSystem::PublicStoreDefs::MaterialId materialId) const
{
    return renderGroupExists(RenderGroupKey(materialId));
}
