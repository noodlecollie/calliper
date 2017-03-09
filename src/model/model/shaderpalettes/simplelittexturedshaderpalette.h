#ifndef SIMPLELITTEXTUREDSHADERPALETTE_H
#define SIMPLELITTEXTUREDSHADERPALETTE_H

#include "model_global.h"
#include "renderer/shaders/baseshaderpalette.h"

namespace Model
{
    class SimpleLitTexturedShaderPalette : public Renderer::BaseShaderPalette
    {
    public:
        SimpleLitTexturedShaderPalette();

        virtual quint16 shader(Renderer::ShaderDefs::ShaderTechnique technique) const override;
    };
}

#endif // SIMPLELITTEXTUREDSHADERPALETTE_H
