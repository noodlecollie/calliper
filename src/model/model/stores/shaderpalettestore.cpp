#include "shaderpalettestore.h"

namespace Model
{
    ShaderPaletteStore::ShaderPaletteStore()
    {
    }

    Renderer::ShaderPalette ShaderPaletteStore::defaultShaderPalette() const
    {
        return m_DefaultPalette;
    }

    void ShaderPaletteStore::setDefaultShaderPalette(const Renderer::ShaderPalette &palette)
    {
        m_DefaultPalette = palette;
    }

    void ShaderPaletteStore::addShaderPalette(RenderMode renderMode, const Renderer::ShaderPalette &palette)
    {
        m_Palettes[renderMode] = palette;
    }

    Renderer::ShaderPalette ShaderPaletteStore::shaderPalette(RenderMode renderMode) const
    {
        return m_Palettes.value(renderMode, m_DefaultPalette);
    }
}
