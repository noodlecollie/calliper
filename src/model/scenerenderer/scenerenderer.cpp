#include "scenerenderer.h"

namespace NS_MODEL
{
    SceneRenderer::SceneRenderer(NS_RENDERER::IRenderer* renderer, Scene* scene)
        : m_pRenderer(renderer), m_pScene(scene), m_pCamera(nullptr)
    {
        Q_ASSERT_X(m_pRenderer, Q_FUNC_INFO, "Renderer cannot be null");
        Q_ASSERT_X(m_pScene, Q_FUNC_INFO, "Scene cannot be null");
    }

    SceneCamera* SceneRenderer::camera() const
    {
        return m_pCamera;
    }

    void SceneRenderer::setCamera(SceneCamera *camera)
    {
        Q_ASSERT_X(!camera || camera->parentScene() == m_pScene, Q_FUNC_INFO, "Camera must belong to this scene!");
        m_pCamera = camera;
    }

    void SceneRenderer::render()
    {
        if ( !m_pCamera )
            return;

        m_matRecursiveUpdateMatrix.setToIdentity();
        updateObjectRecursive(m_pScene->rootObject());
    }

    void SceneRenderer::updateObjectRecursive(SceneObject *object)
    {
        using namespace NS_RENDERER;

        QMatrix4x4 oldMatrix = m_matRecursiveUpdateMatrix;
        m_matRecursiveUpdateMatrix = object->hierarchy().parentToLocal() * m_matRecursiveUpdateMatrix;

        if ( object->needsRendererUpdate() )
        {
            // TODO: update object and submit update to renderer
        }

        QList<SceneObject*> children = object->childSceneObjects();
        foreach ( SceneObject* child, children )
        {
            updateObjectRecursive(child);
        }

        m_matRecursiveUpdateMatrix = oldMatrix;
    }

    void SceneRenderer::drawAllObjects()
    {
        // TODO: Draw with camera and projection matrices
    }
}
