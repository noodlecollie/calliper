#include "renderutils.h"

#include "rendersystem/private/stores/materialstore/materialstore.h"

#include "rendersystem/private/static-stores/rendermodestore/rendermodestore.h"
#include "rendersystem/private/static-stores/openglshaderstore/openglshaderstore.h"

namespace RenderUtils
{
    PrivateShaderDefs::ShaderId shaderFromMaterial(RenderSystem::ShaderDefs::RenderMode renderMode,
                                                   RenderSystem::MaterialDefs::MaterialId materialId)
    {
        using namespace RenderSystem;

        if ( renderMode == RenderSystem::ShaderDefs::UnknownRenderMode )
        {
            return PrivateShaderDefs::UnknownShaderId;
        }

        QSharedPointer<RenderMaterial> material = MaterialStore::globalInstance()->object(materialId);
        if ( !material )
        {
            return PrivateShaderDefs::UnknownShaderId;
        }

        BaseRenderMode* renderModeObject = RenderModeStore::globalInstance()->object(renderMode);
        if ( !renderModeObject )
        {
            return PrivateShaderDefs::UnknownShaderId;
        }

        return renderModeObject->shaderId(material->shaderStyle());
    }
}
