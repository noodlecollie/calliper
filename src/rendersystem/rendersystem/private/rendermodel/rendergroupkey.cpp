#include "rendergroupkey.h"

#include "rendersystem/endpoints/materialstoreendpoint.h"
#include "rendersystem/endpoints/texturestoreendpoint.h"

RenderGroupKey::RenderGroupKey()
    : shaderStyle(RenderSystem::PublicShaderDefs::UnknownShaderStyle),
      mainTextureId(RenderSystem::PublicStoreDefs::INVALID_TEXTURE_ID),
      secondaryTextureId(RenderSystem::PublicStoreDefs::INVALID_TEXTURE_ID)
{
}

RenderGroupKey::RenderGroupKey(RenderSystem::PublicStoreDefs::MaterialId materialId)
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

bool RenderGroupKey::operator <(const RenderGroupKey& other) const
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
