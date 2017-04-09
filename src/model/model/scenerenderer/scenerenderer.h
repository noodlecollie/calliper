#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "model_global.h"

#include <QOpenGLFramebufferObject>

#include "model/scene/scene.h"
#include "model/camera/scenecamera.h"
#include "renderer/functors/ishaderretrievalfunctor.h"
#include "renderer/functors/itextureretrievalfunctor.h"
#include "renderer/functors/imaterialretrievalfunctor.h"
#include "renderer/rendermodel/0-modellevel/rendermodel.h"
#include "irenderpassclassifier.h"
#include "renderer/framebuffers/openglframebuffer.h"

namespace Model
{
    class ResourceEnvironment;

    class MODELSHARED_EXPORT SceneRenderer
    {
    public:
        SceneRenderer(Scene* scene,
                      Renderer::RenderModel* renderer,
                      QOpenGLFramebufferObject* frameBuffer);

        Renderer::BaseShaderPalette* shaderPalette() const;
        void setShaderPalette(Renderer::BaseShaderPalette* palette);

        QVector3D directionalLight() const;
        void setDirectionalLight(const QVector3D& dir);

        void render(const SceneCamera* camera);
        void render(const QMatrix4x4& worldToCamera, const QMatrix4x4& projection);

    private:
        void updateObjectRecursive(SceneObject* object);
        void drawAllObjects(const QMatrix4x4& worldToCamera, const QMatrix4x4& projection);

        Scene* m_pScene;
        Renderer::RenderModel* m_pRenderer;
        QOpenGLFramebufferObject* m_pFrameBuffer;

        QMatrix4x4  m_matRecursiveUpdateMatrix;
        Renderer::BaseShaderPalette* m_pShaderPalette;
        QVector3D m_vecDirectionalLight;
    };
}

#endif // SCENERENDERER_H
