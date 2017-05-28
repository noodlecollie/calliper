#ifndef PUBLICSHADERDEFS_H
#define PUBLICSHADERDEFS_H

#include "rendersystem_global.h"

#include <QObject>

namespace RenderSystem
{
    namespace PublicShaderDefs
    {
        // Different ways in which we might want to draw geometry.
        // Some other examples might be unlit textured, or
        // refract (for water).
        // These are NOT the same as rendering modes (eg. simple lit):
        // rendering modes act as the mapping from the conceptual shader
        // style to the concrete shader, meaning that depending on what
        // context the rendering is happening in, each shader style can be
        // mapped to the appropriate shader.
        enum ShaderStyle
        {
            UnknownShaderStyle = -1,

            UnlitTextured3D = 0,
            LitTextured3D,
            UnlitPerVertexColor3D,

            TOTAL_SHADER_STYLES,
        };

        enum RenderMode
        {
            UnknownRenderMode = -1,

            BarebonesRenderMode = 0,

            TOTAL_RENDER_MODES
        };
    }
}

#endif // PUBLICSHADERDEFS_H
