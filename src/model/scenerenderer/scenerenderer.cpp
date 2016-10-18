#include "scenerenderer.h"

namespace NS_MODEL
{
    SceneRenderer::SceneRenderer(NS_RENDERER::IShaderRetrievalFunctor* shaderFunctor,
                                 NS_RENDERER::ITextureRetrievalFunctor* textureFunctor,
                                 IRenderPassClassifier* renderPassClassifier,
                                 NS_RENDERER::IRenderer* renderer, Scene* scene)
        : m_pShaderFunctor(shaderFunctor), m_pTextureFunctor(textureFunctor),
          m_pRenderPassClassifier(renderPassClassifier),
          m_pRenderer(renderer), m_pScene(scene),
          m_iDefaultShader(0), m_iDefaultTexture(0)
    {
        Q_ASSERT_X(m_pShaderFunctor, Q_FUNC_INFO, "Shader functor cannot be null");
        Q_ASSERT_X(m_pTextureFunctor, Q_FUNC_INFO, "Texture functor cannot be null");
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
        drawAllObjects(camera->worldToCameraMarix(), camera->lens().projectionMatrix());
    }

    void SceneRenderer::render(const QMatrix4x4 &worldToCamera, const QMatrix4x4 &projection)
    {
        m_matRecursiveUpdateMatrix.setToIdentity();
        updateObjectRecursive(m_pScene->rootObject());
        drawAllObjects(worldToCamera, projection);
    }

    void SceneRenderer::updateObjectRecursive(SceneObject *object)
    {
        using namespace NS_RENDERER;

        QMatrix4x4 oldMatrix = m_matRecursiveUpdateMatrix;
        m_matRecursiveUpdateMatrix = object->hierarchy().parentToLocal() * m_matRecursiveUpdateMatrix;

        if ( object->needsRendererUpdate() )
        {
            GeometryBuilder builder(m_pShaderFunctor, m_pTextureFunctor,
                                    m_iDefaultShader, 0,
                                    m_matRecursiveUpdateMatrix);
            object->rendererUpdate(builder);

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
        using namespace NS_RENDERER;
        m_pRenderer->draw(RendererDrawParams(worldToCamera, projection));
    }

    quint16 SceneRenderer::defaultShaderId() const
    {
        return m_iDefaultShader;
    }

    void SceneRenderer::setDefaultShaderId(quint16 id)
    {
        m_iDefaultShader = id;
    }

    quint32 SceneRenderer::defaultTextureId() const
    {
        return m_iDefaultTexture;
    }

    void SceneRenderer::setDefaultTextureId(quint32 id)
    {
        m_iDefaultTexture = id;
    }
}
