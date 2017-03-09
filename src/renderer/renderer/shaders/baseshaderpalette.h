#ifndef BASESHADERPALETTE_H
#define BASESHADERPALETTE_H

#include "renderer_global.h"
#include <QHash>

namespace Renderer
{
    class RENDERERSHARED_EXPORT BaseShaderPalette
    {
    public:
        // Different ways in which we might want to draw geometry.
        enum ShaderTechnique
        {
            LitTextured = 0,
            UnlitPerVertexColor,
        };

        BaseShaderPalette();
        virtual ~BaseShaderPalette();

        // This gives us the ID of the shader that corresponds to this technique
        // within the current rendering mode.
        virtual quint16 shader(ShaderTechnique technique) const = 0;
    };
}

#endif // BASESHADERPALETTE_H
