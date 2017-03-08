#include "scenerenderer.h"
#include "model/global/resourceenvironment.h"

namespace Model
{
    SceneRenderer::SceneRenderer(IRenderPassClassifier* renderPassClassifier,
                                 Renderer::RenderModel* renderer,
                                 Scene* scene)
        : m_pRenderPassClassifier(renderPassClassifier),
          m_pRenderer(renderer),
          m_pScene(scene),
          m_vecDirectionalLight(QVector3D(1,1,1).normalized())
    {
        Q_ASSERT_X(m_pRenderPassClassifier, Q_FUNC_INFO, "Render pass classifier cannot be null");
        Q_ASSERT_X(m_pRenderer, Q_FUNC_INFO, "Renderer cannot be null");
        Q_ASSERT_X(m_pScene, Q_FUNC_INFO, "Scene cannot be null");
    }

    void SceneRenderer::render(const SceneCamera *camera)
    {
        if ( !camera )
            return;

        m_matRecursiveUpdateMatrix.setToIdentity();
        updateObjectRecursive(m_pScene->rootObject());
        drawAllObjects(camera->rootToLocalMatrix(), camera->lens().projectionMatrix());
    }

    void SceneRenderer::render(const QMatrix4x4 &worldToCamera, const QMatrix4x4 &projection)
    {
        m_matRecursiveUpdateMatrix.setToIdentity();
        updateObjectRecursive(m_pScene->rootObject());
        drawAllObjects(worldToCamera, projection);
    }

    void SceneRenderer::updateObjectRecursive(SceneObject *object)
    {
        using namespace Renderer;
        ResourceEnvironment* resourceEnv = ResourceEnvironment::globalInstance();

        QMatrix4x4 oldMatrix = m_matRecursiveUpdateMatrix;
        m_matRecursiveUpdateMatrix = object->hierarchy().parentToLocal() * m_matRecursiveUpdateMatrix;

        if ( object->needsRendererUpdate() )
        {
            GeometryBuilder builder(resourceEnv->shaderStore(),
                                    resourceEnv->textureStore(),
                                    m_ShaderPalette.shader(ShaderPalette::LitTextured),   // TODO: Get this from material instead
                                    0,
                                    m_matRecursiveUpdateMatrix);
            object->rendererUpdate(m_ShaderPalette, builder);

            m_pRenderer->updateObject(
                RendererInputObjectParams(
                    object->objectId(),
                    m_pRenderPassClassifier->classify(object->objectId()),
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

    Renderer::ShaderPalette SceneRenderer::shaderPalette() const
    {
        return m_ShaderPalette;
    }

    void SceneRenderer::setShaderPalette(const Renderer::ShaderPalette &palette)
    {
        m_ShaderPalette = palette;
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
