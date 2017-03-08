#include "shaderpalette.h"

namespace Renderer
{
    ShaderPalette::ShaderPalette()
    {

    }

    void ShaderPalette::addItem(ShaderTechnique item, quint16 shader)
    {
        m_ItemTable.insert(item, shader);
    }

    quint16 ShaderPalette::shader(ShaderTechnique item) const
    {
        return m_ItemTable.value(item, 0);
    }
}
