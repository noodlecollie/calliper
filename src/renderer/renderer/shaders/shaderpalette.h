#ifndef SHADERPALETTE_H
#define SHADERPALETTE_H

#include "renderer_global.h"
#include <QHash>

namespace Renderer
{
    class RENDERERSHARED_EXPORT ShaderPalette
    {
    public:
        enum ShaderTechnique
        {
            LitTextured = 0,
            UnlitPerVertexColor,
        };

        ShaderPalette();

        void addItem(ShaderTechnique item, quint16 shader);
        quint16 shader(ShaderTechnique item) const;

    private:
        QHash<ShaderTechnique, quint16> m_ItemTable;
    };
}

#endif // SHADERPALETTE_H
