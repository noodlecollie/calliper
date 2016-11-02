#include "shaderpalette.h"

namespace Model
{
    ShaderPalette::ShaderPalette()
    {

    }

    void ShaderPalette::addItem(PaletteItem item, quint16 shader)
    {
        m_ItemTable.insert(item, shader);
    }

    quint16 ShaderPalette::shader(PaletteItem item) const
    {
        return m_ItemTable.value(item, 0);
    }
}
