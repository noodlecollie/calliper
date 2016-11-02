#include "basicscene.h"
#include "camera/scenecamera.h"
#include "sceneobjects/originmarker.h"

namespace Model
{
    BasicScene::BasicScene(ShaderStore* shaderStore, TextureStore* textureStore, QObject* parent)
        : Scene(shaderStore, textureStore, parent)
    {
        m_pDefaultCamera = createSceneObject<SceneCamera>(rootObject());
        m_pDefaultCamera->hierarchy().setPosition(QVector3D(-64,0,0));
        m_pDefaultCamera->setObjectName("_defaultCamera");

        m_pOriginMarker = createSceneObject<OriginMarker>(rootObject());
        m_pOriginMarker->setObjectName("_originMarker");
    }

    SceneCamera* BasicScene::defaultCamera() const
    {
        return m_pDefaultCamera;
    }
}
