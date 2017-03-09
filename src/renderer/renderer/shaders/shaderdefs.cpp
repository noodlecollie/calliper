#include "shaderdefs.h"

namespace Renderer
{
    const char* ShaderDefs::GLOBAL_UNIFORM_BLOCK_NAME = "GlobalUniformBlock";
    const char* ShaderDefs::LOCAL_UNIFORM_BLOCK_NAME = "LocalUniformBlock";

    ShaderDefs::VertexFormatUpperBound ShaderDefs::shaderMaxVertexFormat(ShaderTechnique technique)
    {
        switch ( technique )
        {
            case UnlitTextured3D:
            {
                return VertexFormatUpperBound(4, 3, 4, 2);
            }

            case LitTextured3D:
            {
                return VertexFormatUpperBound(4, 3, 4, 2);
            }

            case UnlitPerVertexColor3D:
            {
                return VertexFormatUpperBound(4, 3, 4, 0);
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "No vertex format specified for this shader technique!");
                return VertexFormatUpperBound(0, 0, 0, 0);
            }
        }
    }
}
