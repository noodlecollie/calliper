#include "shaderpalettestore.h"
#include "model/shaderpalettes/simplelittexturedshaderpalette.h"

namespace Model
{
    ShaderPaletteStore::ShaderPaletteStore()
    {
        memset(m_Palettes, 0, TOTAL_RENDER_MODES * sizeof(Renderer::BaseShaderPalette*));

        createPalettes();
    }

    ShaderPaletteStore::~ShaderPaletteStore()
    {
        for ( int i = 0; i < TOTAL_RENDER_MODES; ++i )
        {
            delete m_Palettes[i];
            m_Palettes[i] = Q_NULLPTR;
        }
    }

    void ShaderPaletteStore::createPalettes()
    {
        addShaderPalette(SimpleLitTexturedRenderMode, new SimpleLitTexturedShaderPalette());
    }

    Renderer::BaseShaderPalette* ShaderPaletteStore::shaderPalette(RenderMode renderMode) const
    {
        if ( renderMode == UnknownRenderMode || renderMode >= TOTAL_RENDER_MODES )
        {
            return Q_NULLPTR;
        }

        return m_Palettes[renderMode];
    }

    void ShaderPaletteStore::addShaderPalette(RenderMode renderMode, Renderer::BaseShaderPalette* palette)
    {
        Q_ASSERT_X(!m_Palettes[renderMode], Q_FUNC_INFO, "Palette for this render mode already exists!");

        if ( m_Palettes[renderMode] )
        {
            delete m_Palettes[renderMode];
        }

        m_Palettes[renderMode] = palette;
    }
}
