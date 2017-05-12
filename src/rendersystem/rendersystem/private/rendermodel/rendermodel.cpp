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
    CurrentContextGuard<ITextureStore> textureStore = TextureStoreEndpoint::textureStore();

    QSharedPointer<RenderMaterial> material = materialStore->material(materialId).toStrongRef();

    shaderStyle = material->shaderStyle();
    mainTextureId = textureStore->textureIdFromPath(material->textureMapping(PublicTextureDefs::MainTexture));
    secondaryTextureId = textureStore->textureIdFromPath(material->textureMapping(PublicTextureDefs::SecondaryTexture));
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
