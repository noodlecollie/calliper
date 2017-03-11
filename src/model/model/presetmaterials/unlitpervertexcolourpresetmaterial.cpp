#include "unlitpervertexcolourpresetmaterial.h"

namespace Model
{
    UnlitPerVertexColourPresetMaterial::UnlitPerVertexColourPresetMaterial(quint32 id, const QString &path)
        : Renderer::RenderMaterial(id, path)
    {
        setShaderTechnique(Renderer::ShaderDefs::UnlitPerVertexColor3D);
    }
}
