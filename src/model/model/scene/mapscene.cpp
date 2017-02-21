#include "mapscene.h"
#include "model/camera/scenecamera.h"
#include "model/sceneobjects/originmarker.h"

namespace Model
{
    MapScene::MapScene(QObject* parent)
        : Scene(parent)
    {
        m_pDefaultCamera = createSceneObject<SceneCamera>(rootObject());
        m_pDefaultCamera->hierarchy().setPosition(QVector3D(-64,0,0));
        m_pDefaultCamera->setObjectName("_defaultCamera");

        m_pOriginMarker = createSceneObject<OriginMarker>(rootObject());
        m_pOriginMarker->setObjectName("_originMarker");
    }

    SceneCamera* MapScene::defaultCamera() const
    {
        return m_pDefaultCamera;
    }

    QList<SceneCamera*> MapScene::cameras() const
    {
        return rootObject()->findChildren<SceneCamera*>();
    }
}
