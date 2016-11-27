#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "model_global.h"
#include "scene/scene.h"
#include "irenderer.h"
#include "camera/scenecamera.h"
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"
#include "functors/imaterialretrievalfunctor.h"
#include "irenderpassclassifier.h"

namespace Model
{
    class MODELSHARED_EXPORT SceneRenderer
    {
    public:
        SceneRenderer(Renderer::IShaderRetrievalFunctor* shaderFunctor,
                      Renderer::ITextureRetrievalFunctor* textureFunctor,
                      Renderer::IMaterialRetrievalFunctor* materialFunctor,
                      IRenderPassClassifier* renderPassClassifier,
                      Renderer::IRenderer* renderer, Scene* scene);

        quint32 defaultTextureId() const;
        void setDefaultTextureId(quint32 id);

        ShaderPalette shaderPalette() const;
        void setShaderPalette(const ShaderPalette& palette);

        QVector3D directionalLight() const;
        void setDirectionalLight(const QVector3D& dir);

        void render(const SceneCamera* camera);
        void render(const QMatrix4x4& worldToCamera, const QMatrix4x4& projection);

    private:
        void updateObjectRecursive(SceneObject* object);
        void drawAllObjects(const QMatrix4x4& worldToCamera, const QMatrix4x4& projection);

        Renderer::IShaderRetrievalFunctor* m_pShaderFunctor;
        Renderer::ITextureRetrievalFunctor* m_pTextureFunctor;
        Renderer::IMaterialRetrievalFunctor* m_pMaterialFunctor;
        IRenderPassClassifier* m_pRenderPassClassifier;
        Renderer::IRenderer* m_pRenderer;
        Scene* m_pScene;
        quint32 m_iDefaultTexture;

        QMatrix4x4  m_matRecursiveUpdateMatrix;
        ShaderPalette m_ShaderPalette;
        QVector3D m_vecDirectionalLight;
    };
}

#endif // SCENERENDERER_H
