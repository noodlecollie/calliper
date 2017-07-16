#ifndef PUBLICSHADERDEFS_H
#define PUBLICSHADERDEFS_H

#include "rendersystem_global.h"

namespace RenderSystem
{
    namespace ShaderDefs
    {
        enum ShaderId
        {
            UnknownShaderId = -1,

            ErrorShaderId = 0,
            LitGenericShaderId,
            UnlitGenericShaderId,
            DebugMinimalShaderId,

            TOTAL_SHADERS
        };

        enum RenderMode
        {
            UnknownRenderMode = -1,

            BarebonesRenderMode = 0,

            DebugMinimalMode,

            TOTAL_RENDER_MODES
        };
    }
}

#endif // PUBLICSHADERDEFS_H
