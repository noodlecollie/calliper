#include "barebonesrendermode.h"

BarebonesRenderMode::BarebonesRenderMode()
    : BaseRenderMode(RenderSystem::PublicShaderDefs::BarebonesRenderMode)
{

}

PrivateShaderDefs::ShaderId BarebonesRenderMode::shaderId(RenderSystem::PublicShaderDefs::ShaderStyle style) const
{
    using namespace RenderSystem;

    switch ( style )
    {
        case PublicShaderDefs::LitTextured3D:
        {
            return PrivateShaderDefs::SimpleLitShaderId;
        }

        case PublicShaderDefs::UnlitPerVertexColor3D:
        {
            return PrivateShaderDefs::UnlitPerVertexColourShaderId;
        }

        default:
        {
            return PrivateShaderDefs::UnknownShaderId;
        }
    }
}
