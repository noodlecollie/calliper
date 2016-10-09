#ifndef IRENDERER_H
#define IRENDERER_H

#include "renderer_global.h"
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"
#include "rendermodel/rendererinputobjectparams.h"
#include "rendermodel/rendererdrawparams.h"
#include "rendermodel/rendererobjectflags.h"

namespace NS_RENDERER
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() {}

        // Shader functor - for getting shader program objects given a shader ID.
        virtual IShaderRetrievalFunctor* shaderFunctor() = 0;
        virtual void setShaderFunctor(IShaderRetrievalFunctor* functor) = 0;

        // Texture functor - for getting texture objects given a texture ID.
        virtual ITextureRetrievalFunctor* textureFunctor() = 0;
        virtual void setTextureFunctor(ITextureRetrievalFunctor* functor) = 0;

        // Object modification properties. These are independent of rendering frames.
        virtual void updateObject(const RendererInputObjectParams &object) = 0;
        virtual void removeObject(quint32 objectId) = 0;

        // These are set using bitwise OR, not just assignment.
        virtual void setObjectFlags(quint32 objectId, quint32 flags) = 0;
        virtual void clearObjectFlags(quint32 objectId, quint32 flags) = 0;
        virtual quint32 getObjectFlags(quint32 objectId) const = 0;

        // Draw the specified objects.
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
