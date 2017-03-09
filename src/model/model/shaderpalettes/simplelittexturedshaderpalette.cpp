#include "simplelittexturedshaderpalette.h"
#include "model/shaders/knownshaderdefs.h"

namespace Model
{
    SimpleLitTexturedShaderPalette::SimpleLitTexturedShaderPalette()
    {

    }

    quint16 SimpleLitTexturedShaderPalette::shader(ShaderTechnique technique) const
    {
        switch ( technique )
        {
            case BaseShaderPalette::LitTextured:
            {
                return KnownShaderDefs::SimpleLitShaderId;
            }

            case BaseShaderPalette::UnlitPerVertexColor:
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
