#include "scenerenderer.h"
#include "model/global/resourceenvironment.h"
#include "renderer/opengl/scopedcurrentcontext.h"
#include "calliperutil/debug/debug.h"

namespace Model
{
    SceneRenderer::SceneRenderer(Scene* scene,
                                 Renderer::RenderModel* renderer,
                                 QOpenGLFramebufferObject* frameBuffer)
        : m_pScene(scene),
          m_pRenderer(renderer),
          m_pFrameBuffer(frameBuffer),
          m_matRecursiveUpdateMatrix(),
          m_pShaderPalette(Q_NULLPTR),
          m_vecDirectionalLight(QVector3D(1,1,1).normalized())
    {
        Q_ASSERT_X(m_pScene, Q_FUNC_INFO, "Scene cannot be null");
        Q_ASSERT_X(m_pRenderer, Q_FUNC_INFO, "Renderer cannot be null");
        Q_ASSERT_X(m_pFrameBuffer, Q_FUNC_INFO, "Frame buffer cannot be null");
    }

    void SceneRenderer::render(const SceneCamera *camera)
    {
        if ( !camera )
        {
            return;
        }

        render(camera->rootToLocalMatrix(), camera->lens().projectionMatrix());
    }

    void SceneRenderer::render(const QMatrix4x4 &worldToCamera, const QMatrix4x4 &projection)
    {
        Q_ASSERT(m_pShaderPalette);
        if ( !m_pShaderPalette )
        {
            return;
        }

        Renderer::ScopedCurrentContext scopedContext;
        Q_UNUSED(scopedContext);

        CUTL_ASSERT_SUCCESS(m_pFrameBuffer->bind());

        m_matRecursiveUpdateMatrix.setToIdentity();
        updateObjectRecursive(m_pScene->rootObject());
        drawAllObjects(worldToCamera, projection);

        CUTL_ASSERT_SUCCESS(m_pFrameBuffer->release());
    }

    void SceneRenderer::updateObjectRecursive(SceneObject *object)
    {
        using namespace Renderer;
        ResourceEnvironment* resourceEnv = ResourceEnvironment::globalInstance();

        QMatrix4x4 oldMatrix = m_matRecursiveUpdateMatrix;
        m_matRecursiveUpdateMatrix = object->hierarchy().parentToLocal() * m_matRecursiveUpdateMatrix;

        if ( object->needsRendererUpdate() )
        {
            GeometryBuilder builder(resourceEnv->renderFunctors(),
                                    m_pShaderPalette,
                                    0,
                                    m_matRecursiveUpdateMatrix);
            object->rendererUpdate(builder);

            m_pRenderer->updateObject(
                RendererInputObjectParams(
                    object->objectId(),
                    m_pScene->classify(object->objectId()),
                    builder
                )
            );
        }

        QList<SceneObject*> children = object->childSceneObjects();
        foreach ( SceneObject* child, children )
        {
            updateObjectRecursive(child);
        }

        m_matRecursiveUpdateMatrix = oldMatrix;
    }

    void SceneRenderer::drawAllObjects(const QMatrix4x4 &worldToCamera, const QMatrix4x4 &projection)
    {
        using namespace Renderer;

        RendererDrawParams params;
        params.setWorldToCameraMatrix(worldToCamera);
        params.setProjectionMatrix(projection);
        params.setDirectionalLight(m_vecDirectionalLight);

        m_pRenderer->draw(params);
    }

    Renderer::BaseShaderPalette* SceneRenderer::shaderPalette() const
    {
        return m_pShaderPalette;
    }

    void SceneRenderer::setShaderPalette(Renderer::BaseShaderPalette* palette)
    {
        m_pShaderPalette = palette;
    }

    QVector3D SceneRenderer::directionalLight() const
    {
        return m_vecDirectionalLight;
    }

    void SceneRenderer::setDirectionalLight(const QVector3D &dir)
    {
        m_vecDirectionalLight = dir;
    }
}
