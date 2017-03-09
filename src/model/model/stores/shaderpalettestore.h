#ifndef SHADERPALETTESTORE_H
#define SHADERPALETTESTORE_H

#include "model_global.h"
#include "renderer/shaders/baseshaderpalette.h"

namespace Model
{
    class MODELSHARED_EXPORT ShaderPaletteStore
    {
    public:
        // Modes of rendering. Each mode uses a different shader palette,
        // which will map known shader techniques to different shaders
        // depending on the mode.
        enum RenderMode
        {
            UnknownRenderMode = 0,

            SimpleLitTexturedRenderMode,

            TOTAL_RENDER_MODES
        };

        ShaderPaletteStore();
        ~ShaderPaletteStore();

        Renderer::BaseShaderPalette* shaderPalette(RenderMode renderMode) const;

    private:
        void createPalettes();
        void addShaderPalette(RenderMode renderMode, Renderer::BaseShaderPalette* palette);

        Renderer::BaseShaderPalette* m_Palettes[TOTAL_RENDER_MODES];
    };
}

#endif // SHADERPALETTESTORE_H
