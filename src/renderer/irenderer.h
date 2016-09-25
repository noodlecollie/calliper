#ifndef IRENDERER_H
#define IRENDERER_H

#include "renderer_global.h"
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"
#include "rendermodel/rendererinputobjectparams.h"
#include "rendermodel/rendererdrawparams.h"

namespace NS_RENDERER
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() {}

        enum RenderPasses
        {
            PASS_GENERAL,   // Standard, opaque objects.

            TOTAL_RENDER_PASSES
        };

        virtual IShaderRetrievalFunctor* shaderFunctor() = 0;
        virtual void setShaderFunctor(IShaderRetrievalFunctor* functor) = 0;

        virtual ITextureRetrievalFunctor* textureFunctor() = 0;
        virtual void setTextureFunctor(ITextureRetrievalFunctor* functor) = 0;

        virtual void updateObject(const RendererInputObjectParams &object) = 0;

        virtual void draw(const RendererDrawParams &params) = 0;
    };

    namespace Global
    {
        RENDERERSHARED_EXPORT void initialise();
        RENDERERSHARED_EXPORT void shutdown();
        RENDERERSHARED_EXPORT IRenderer* renderer();
    }
}

#endif // IRENDERER_H
