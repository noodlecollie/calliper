#include "privateshaderdefs.h"
#include "vertexformat.h"

namespace PrivateShaderDefs
{
    const char* GLOBAL_UNIFORM_BLOCK_NAME = "GlobalUniformBlock";
    const char* LOCAL_UNIFORM_BLOCK_NAME = "LocalUniformBlock";

    VertexFormatUpperBound shaderMaxVertexFormat(RenderSystem::ShaderDefs::ShaderStyle style)
    {
        using namespace RenderSystem;

        switch ( style )
        {
            case ShaderDefs::UnlitTextured3D:
            {
                return VertexFormatUpperBound(4, 3, 4, 2);
            }

            case ShaderDefs::LitTextured3D:
            {
                return VertexFormatUpperBound(4, 3, 4, 2);
            }

            case ShaderDefs::UnlitPerVertexColor3D:
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
