#ifndef SHADERPALETTE_H
#define SHADERPALETTE_H

#include "model_global.h"
#include <QHash>

namespace NS_MODEL
{
    class MODELSHARED_EXPORT ShaderPalette
    {
    public:
        enum PaletteItem
        {
            DefaultShader = 0,
            UnlitPerVertexColor,
        };

        ShaderPalette();

        void addItem(PaletteItem item, quint16 shader);
        quint16 shader(PaletteItem item) const;

    private:
        QHash<PaletteItem, quint16> m_ItemTable;
    };
}

#endif // SHADERPALETTE_H
