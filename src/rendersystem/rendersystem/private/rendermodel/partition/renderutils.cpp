#include "renderutils.h"

#include "rendersystem/private/stores/materialstore/materialstore.h"
#include "rendersystem/private/stores/rendermodestore/rendermodestore.h"
#include "rendersystem/private/stores/openglshaderstore/openglshaderstore.h"

namespace RenderUtils
{
    OpenGLShaderProgram* shaderFromMaterial(RenderSystem::PublicShaderDefs::RenderMode renderMode,
                                            RenderSystem::PublicStoreDefs::MaterialId materialId)
    {
        using namespace RenderSystem;

        if ( renderMode == RenderSystem::PublicShaderDefs::UnknownRenderMode ||
             materialId == PublicStoreDefs::INVALID_MATERIAL_ID )
        {
            return Q_NULLPTR;
        }

        QSharedPointer<RenderMaterial> material = MaterialStore::globalInstance()->object(materialId);
        if ( !material )
        {
            return Q_NULLPTR;
        }

        PublicShaderDefs::ShaderStyle shaderStyle = material->shaderStyle();
        if ( shaderStyle == PublicShaderDefs::UnknownShaderStyle )
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
