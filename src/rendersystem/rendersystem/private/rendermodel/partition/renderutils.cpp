#include "renderutils.h"

#include "rendersystem/private/stores/materialstore/materialstore.h"

#include "rendersystem/private/static-stores/rendermodestore/rendermodestore.h"
#include "rendersystem/private/static-stores/openglshaderstore/openglshaderstore.h"

namespace RenderUtils
{
    OpenGLShaderProgram* shaderFromMaterial(RenderSystem::ShaderDefs::RenderMode renderMode,
                                            RenderSystem::MaterialDefs::MaterialId materialId)
    {
        using namespace RenderSystem;
        OpenGLShaderProgram* const errorShader =
                OpenGLShaderStore::globalInstance()->object(PrivateShaderDefs::ErrorShaderId);

        if ( renderMode == RenderSystem::ShaderDefs::UnknownRenderMode )
        {
            return errorShader;
        }

        QSharedPointer<RenderMaterial> material = MaterialStore::globalInstance()->object(materialId);
        if ( !material )
        {
            return errorShader;
        }

        ShaderDefs::ShaderStyle shaderStyle = material->shaderStyle();
        if ( shaderStyle == ShaderDefs::UnknownShaderStyle )
        {
            return errorShader;
        }

        BaseRenderMode* renderModeObject = RenderModeStore::globalInstance()->object(renderMode);
        if ( !renderModeObject )
        {
            return errorShader;
        }

        PrivateShaderDefs::ShaderId shaderId = renderModeObject->shaderId(shaderStyle);
        if ( shaderId == PrivateShaderDefs::UnknownShaderId )
        {
            return errorShader;
        }

        return OpenGLShaderStore::globalInstance()->object(shaderId);
    }
}
