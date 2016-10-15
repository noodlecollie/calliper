#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "model_global.h"
#include "scene/scene.h"
#include "irenderer.h"
#include "camera/scenecamera.h"
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"
#include "irenderpassclassifier.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT SceneRenderer
    {
    public:
        SceneRenderer(NS_RENDERER::IShaderRetrievalFunctor* shaderFunctor,
                      NS_RENDERER::ITextureRetrievalFunctor* textureFunctor,
                      IRenderPassClassifier* renderPassClassifier,
                      NS_RENDERER::IRenderer* renderer, Scene* scene);

        quint16 defaultShaderId() const;
        void setDefaultShaderId(quint16 id);

        quint32 defaultTextureId() const;
        void setDefaultTextureId(quint32 id);

        void render(const SceneCamera* camera);
        void render(const QMatrix4x4& worldToCamera, const QMatrix4x4& projection);

    private:
        void updateObjectRecursive(SceneObject* object);
        void drawAllObjects(const QMatrix4x4& worldToCamera, const QMatrix4x4& projection);

        NS_RENDERER::IShaderRetrievalFunctor* m_pShaderFunctor;
        NS_RENDERER::ITextureRetrievalFunctor* m_pTextureFunctor;
        IRenderPassClassifier* m_pRenderPassClassifier;
        NS_RENDERER::IRenderer* m_pRenderer;
        Scene* m_pScene;
        quint16 m_iDefaultShader;
        quint32 m_iDefaultTexture;

        QMatrix4x4  m_matRecursiveUpdateMatrix;
    };
}

#endif // SCENERENDERER_H
