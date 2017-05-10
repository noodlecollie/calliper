#ifndef PUBLICSHADERDEFS_H
#define PUBLICSHADERDEFS_H

#include "rendersystem_global.h"

#include <QObject>

namespace RenderSystem
{
    class PublicShaderDefs
    {
        Q_GADGET
        PublicShaderDefs() = delete;
    public:
        // Different ways in which we might want to draw geometry.
        // Some other examples might be unlit textured, or
        // refract (for water).
        enum ShaderStyle
        {
            UnknownShaderStyle = -1,

            UnlitTextured3D = 0,
            LitTextured3D,
            UnlitPerVertexColor3D,

            TOTAL_SHADER_STYLES,

            // The lower byte of a quint16 is reserved for shader styles.
            // The upper byte is for individual shader overrides.
            MAX_SHADER_STYLES = 0x00ff,
        };
        Q_ENUM(ShaderStyle)
    };
}

#endif // PUBLICSHADERDEFS_H
