#ifndef RENDERFUNCTORGROUP_H
#define RENDERFUNCTORGROUP_H

#include "renderer_global.h"
#include "ishaderretrievalfunctor.h"
#include "itextureretrievalfunctor.h"
#include "imaterialretrievalfunctor.h"

namespace Renderer
{
    struct RenderFunctorGroup
    {
        IShaderRetrievalFunctor* shaderFunctor;
        ITextureRetrievalFunctor* textureFunctor;
        IMaterialRetrievalFunctor* materialFunctor;

        RenderFunctorGroup(IShaderRetrievalFunctor* shader,
                           ITextureRetrievalFunctor* texture,
                           IMaterialRetrievalFunctor* material)
            : shaderFunctor(shader),
              textureFunctor(texture),
              materialFunctor(material)
        {
        }

        RenderFunctorGroup()
            : shaderFunctor(Q_NULLPTR),
              textureFunctor(Q_NULLPTR),
              materialFunctor(Q_NULLPTR)
        {
        }

        inline bool isValid() const
        {
            return shaderFunctor && textureFunctor && materialFunctor;
        }
    };
}

#endif // RENDERFUNCTORGROUP_H
