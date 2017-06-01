#include "renderutils.h"

#include "rendersystem/private/stores/materialstore/materialstore.h"
#include "rendersystem/private/stores/rendermodestore/rendermodestore.h"
#include "rendersystem/private/stores/openglshaderstore/openglshaderstore.h"

namespace RenderUtils
{
    OpenGLShaderProgram* shaderFromMaterial(RenderSystem::ShaderDefs::RenderMode renderMode,
                                            RenderSystem::MaterialDefs::MaterialId materialId)
    {
        using namespace RenderSystem;

        if ( renderMode == RenderSystem::ShaderDefs::UnknownRenderMode ||
             materialId == MaterialDefs::INVALID_MATERIAL_ID )
        {
            return Q_NULLPTR;
        }

        QSharedPointer<RenderMaterial> material = MaterialStore::globalInstance()->object(materialId);
        if ( !material )
        {
            return Q_NULLPTR;
        }

        ShaderDefs::ShaderStyle shaderStyle = material->shaderStyle();
        if ( shaderStyle == ShaderDefs::UnknownShaderStyle )
        {
            return Q_NULLPTR;
        }

        BaseRenderMode* renderModeObject = RenderModeStore::globalInstance()->object(renderMode);
        if ( !renderModeObject )
        {
            return Q_NULLPTR;
        }

        PrivateShaderDefs::ShaderId shaderId = renderModeObject->shaderId(shaderStyle);
        if ( shaderId == PrivateShaderDefs::UnknownShaderId )
        {
            return Q_NULLPTR;
        }

        return OpenGLShaderStore::globalInstance()->object(shaderId);
    }
}
