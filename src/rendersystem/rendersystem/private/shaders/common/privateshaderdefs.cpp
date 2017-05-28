#include "privateshaderdefs.h"

namespace PrivateShaderDefs
{
    const char* GLOBAL_UNIFORM_BLOCK_NAME = "GlobalUniformBlock";
    const char* LOCAL_UNIFORM_BLOCK_NAME = "LocalUniformBlock";

    VertexFormatUpperBound shaderMaxVertexFormat(RenderSystem::PublicShaderDefs::ShaderStyle style)
    {
        using namespace RenderSystem;

        switch ( style )
        {
            case PublicShaderDefs::UnlitTextured3D:
            {
                return VertexFormatUpperBound(4, 3, 4, 2);
            }

            case PublicShaderDefs::LitTextured3D:
            {
                return VertexFormatUpperBound(4, 3, 4, 2);
            }

            case PublicShaderDefs::UnlitPerVertexColor3D:
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
