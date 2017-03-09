#include "simplelittexturedshaderpalette.h"
#include "model/shaders/knownshaderdefs.h"

namespace Model
{
    SimpleLitTexturedShaderPalette::SimpleLitTexturedShaderPalette()
    {

    }

    quint16 SimpleLitTexturedShaderPalette::shader(Renderer::ShaderDefs::ShaderTechnique technique) const
    {
        switch ( technique )
        {
            case Renderer::ShaderDefs::LitTextured3D:
            {
                return KnownShaderDefs::SimpleLitShaderId;
            }

            case Renderer::ShaderDefs::UnlitPerVertexColor3D:
            {
                return KnownShaderDefs::UnlitPerVertexColourShaderId;
            }

            default:
            {
                return KnownShaderDefs::UnknownShaderId;
            }
        }
    }
}
