#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "model_global.h"
#include "model/scene/scene.h"
#include "model/camera/scenecamera.h"
#include "renderer/functors/ishaderretrievalfunctor.h"
#include "renderer/functors/itextureretrievalfunctor.h"
#include "renderer/functors/imaterialretrievalfunctor.h"
#include "renderer/rendermodel/0-modellevel/rendermodel.h"
#include "irenderpassclassifier.h"

namespace Model
{
    class ResourceEnvironment;

    class MODELSHARED_EXPORT SceneRenderer
    {
    public:
        // TODO: Make so that things can be set outside of constructor, and handle when they're null.
        SceneRenderer(IRenderPassClassifier* renderPassClassifier,
                      Renderer::RenderModel* renderer,
                      Scene* scene);
        SceneRenderer() {/*REMOVE ME*/}

        Renderer::ShaderPalette shaderPalette() const;
        void setShaderPalette(const Renderer::ShaderPalette& palette);

        QVector3D directionalLight() const;
        void setDirectionalLight(const QVector3D& dir);

        void render(const SceneCamera* camera);
        void render(const QMatrix4x4& worldToCamera, const QMatrix4x4& projection);

    private:
        void updateObjectRecursive(SceneObject* object);
        void drawAllObjects(const QMatrix4x4& worldToCamera, const QMatrix4x4& projection);

        IRenderPassClassifier* m_pRenderPassClassifier;
        Renderer::RenderModel* m_pRenderer;
        Scene* m_pScene;

        QMatrix4x4  m_matRecursiveUpdateMatrix;
        Renderer::ShaderPalette m_ShaderPalette;
        QVector3D m_vecDirectionalLight;
    };
}

#endif // SCENERENDERER_H
