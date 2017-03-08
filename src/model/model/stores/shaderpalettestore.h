#ifndef SHADERPALETTESTORE_H
#define SHADERPALETTESTORE_H

#include "model_global.h"
#include "renderer/shaders/shaderpalette.h"

namespace Model
{
    class MODELSHARED_EXPORT ShaderPaletteStore
    {
    public:
        enum RenderMode
        {
            TodoImplementMe,
        };

        ShaderPaletteStore();

        Renderer::ShaderPalette shaderPalette(RenderMode renderMode) const;
        void addShaderPalette(RenderMode renderMode, const Renderer::ShaderPalette& palette);

        Renderer::ShaderPalette defaultShaderPalette() const;
        void setDefaultShaderPalette(const Renderer::ShaderPalette& palette);

    private:
        QHash<RenderMode, Renderer::ShaderPalette> m_Palettes;
        Renderer::ShaderPalette m_DefaultPalette;
    };
}

#endif // SHADERPALETTESTORE_H
