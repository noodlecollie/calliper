#include "barebonesrendermode.h"

BarebonesRenderMode::BarebonesRenderMode()
    : BaseRenderMode(RenderSystem::ShaderDefs::BarebonesRenderMode)
{

}

PrivateShaderDefs::ShaderId BarebonesRenderMode::shaderId(RenderSystem::ShaderDefs::ShaderStyle style) const
{
    using namespace RenderSystem;

    switch ( style )
    {
        case ShaderDefs::LitTextured3D:
        {
            return PrivateShaderDefs::SimpleLitShaderId;
        }

        case ShaderDefs::UnlitPerVertexColor3D:
        {
            return PrivateShaderDefs::UnlitPerVertexColourShaderId;
        }

        default:
        {
            return BaseRenderMode::shaderId(style);
        }
    }
}
