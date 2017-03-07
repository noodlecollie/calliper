#ifndef SHADERPALETTESTORE_H
#define SHADERPALETTESTORE_H

#include "model_global.h"
#include "model/shaders/shaderpalette.h"

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

        ShaderPalette shaderPalette(RenderMode renderMode) const;
        void addShaderPalette(RenderMode renderMode, const ShaderPalette& palette);

        ShaderPalette defaultShaderPalette() const;
        void setDefaultShaderPalette(const ShaderPalette& palette);

    private:
        QHash<RenderMode, ShaderPalette> m_Palettes;
        ShaderPalette m_DefaultPalette;
    };
}

#endif // SHADERPALETTESTORE_H
