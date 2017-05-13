#ifndef RENDERGROUPKEY_H
#define RENDERGROUPKEY_H

#include "rendersystem/interface-classes/shader/publicshaderdefs.h"
#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

struct RenderGroupKey
{
    RenderSystem::PublicShaderDefs::ShaderStyle shaderStyle;
    RenderSystem::PublicStoreDefs::TextureId mainTextureId;
    RenderSystem::PublicStoreDefs::TextureId secondaryTextureId;

    RenderGroupKey();
    RenderGroupKey(RenderSystem::PublicStoreDefs::MaterialId materialId);
    bool operator <(const RenderGroupKey& other) const;
};

#endif // RENDERGROUPKEY_H
