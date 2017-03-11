#ifndef UNLITPERVERTEXCOLOURPRESETMATERIAL_H
#define UNLITPERVERTEXCOLOURPRESETMATERIAL_H

#include "model_global.h"
#include "renderer/materials/rendermaterial.h"

namespace Model
{
    class UnlitPerVertexColourPresetMaterial : public Renderer::RenderMaterial
    {
    public:
        UnlitPerVertexColourPresetMaterial(quint32 id, const QString& path);
    };
}

#endif // UNLITPERVERTEXCOLOURPRESETMATERIAL_H
