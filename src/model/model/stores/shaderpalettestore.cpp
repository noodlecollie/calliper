#include "shaderpalettestore.h"

namespace Model
{
    ShaderPaletteStore::ShaderPaletteStore()
    {
    }

    ShaderPalette ShaderPaletteStore::defaultShaderPalette() const
    {
        return m_DefaultPalette;
    }

    void ShaderPaletteStore::setDefaultShaderPalette(const ShaderPalette &palette)
    {
        m_DefaultPalette = palette;
    }

    void ShaderPaletteStore::addShaderPalette(RenderMode renderMode, const ShaderPalette &palette)
    {
        m_Palettes[renderMode] = palette;
    }

    ShaderPalette ShaderPaletteStore::shaderPalette(RenderMode renderMode) const
    {
        return m_Palettes.value(renderMode, m_DefaultPalette);
    }
}
