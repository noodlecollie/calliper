#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "model_global.h"
#include "scene/scene.h"
#include "irenderer.h"
#include "sceneobjects/scenecamera.h"
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"

namespace NS_MODEL
{
    class SceneRenderer
    {
    public:
        SceneRenderer(NS_RENDERER::IShaderRetrievalFunctor* shaderFunctor,
                      NS_RENDERER::ITextureRetrievalFunctor* textureFunctor,
                      NS_RENDERER::IRenderer* renderer, Scene* scene);

        SceneCamera* camera() const;
        void setCamera(SceneCamera* camera);

        void render();

    private:
        void updateObjectRecursive(SceneObject* object);
        void drawAllObjects();

        NS_RENDERER::IShaderRetrievalFunctor* m_pShaderFunctor;
        NS_RENDERER::ITextureRetrievalFunctor* m_pTextureFunctor;
        NS_RENDERER::IRenderer* m_pRenderer;
        Scene* m_pScene;
        SceneCamera* m_pCamera;

        QMatrix4x4  m_matRecursiveUpdateMatrix;
    };
}

#endif // SCENERENDERER_H
