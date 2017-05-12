#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <QMap>

#include "rendergroup.h"

#include "rendersystem/interface-classes/shader/publicshaderdefs.h"
#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

class RenderModel
{
public:
    RenderModel();

private:
    struct RenderGroupKey
    {
        RenderSystem::PublicShaderDefs::ShaderStyle shaderStyle;
        RenderSystem::PublicStoreDefs::TextureId mainTextureId;
        RenderSystem::PublicStoreDefs::TextureId secondaryTextureId;

        RenderGroupKey();
        RenderGroupKey(RenderSystem::PublicStoreDefs::MaterialId materialId);
        bool operator <(const RenderGroupKey& other) const;
    };

    bool renderGroupExists(RenderSystem::PublicStoreDefs::MaterialId materialId) const;
    bool renderGroupExists(const RenderGroupKey& key) const;

    QMap<RenderGroupKey, RenderGroup> m_RenderGroups;
};

#endif // RENDERMODEL_H
