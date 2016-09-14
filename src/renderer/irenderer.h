#ifndef IRENDERER_H
#define IRENDERER_H

#include "renderer_global.h"
#include "rendermodel/rendermodelinputparams.h"
#include "geometry/geometrysection.h"
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"

namespace NS_RENDERER
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() {}

        virtual IShaderRetrievalFunctor* shaderFunctor() = 0;
        virtual void setShaderFunctor(IShaderRetrievalFunctor* functor) = 0;

        virtual ITextureRetrievalFunctor* textureFunctor() = 0;
        virtual void setTextureFunctor(ITextureRetrievalFunctor* functor) = 0;

        // Updates the geometry for the given item.
        virtual void updateItem(quint64 objectUniqueId, const RenderModelInputParams &params, const GeometrySectionList &geometry) = 0;

        // Removes the given item, deleting all its geometry.
        virtual void removeItem(quint64 objectUniqueId) = 0;
    };

    void initialise();
    void shutdown();
    IRenderer* renderer();
}

#endif // IRENDERER_H
