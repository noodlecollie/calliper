#include "renderutils.h"

#include "rendersystem/private/stores/materialstore/materialstore.h"

#include "rendersystem/private/static-stores/rendermodestore/rendermodestore.h"
#include "rendersystem/private/static-stores/openglshaderstore/openglshaderstore.h"

namespace RenderUtils
{
    RenderSystem::ShaderDefs::ShaderId shaderFromMaterial(RenderSystem::ShaderDefs::RenderMode renderMode,
                                                   RenderSystem::MaterialDefs::MaterialId materialId)
    {
        using namespace RenderSystem;

        if ( renderMode == RenderSystem::ShaderDefs::UnknownRenderMode )
        {
            return RenderSystem::ShaderDefs::UnknownShaderId;
        }

        QSharedPointer<RenderMaterial> material = MaterialStore::globalInstance()->object(materialId);
        if ( !material )
        {
            return RenderSystem::ShaderDefs::UnknownShaderId;
        }

        BaseRenderMode* renderModeObject = RenderModeStore::globalInstance()->object(renderMode);
        if ( !renderModeObject )
        {
            return RenderSystem::ShaderDefs::UnknownShaderId;
        }

        return renderModeObject->shaderId(material->shaderId());
    }
}
