#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "model_global.h"
#include "scene.h"

namespace Model
{
    class SceneCamera;
    class OriginMarker;

    class MODELSHARED_EXPORT MapScene : public Scene
    {
        Q_OBJECT
    public:
        explicit MapScene(QObject* parent = Q_NULLPTR);

        SceneCamera* defaultCamera() const;
        QList<SceneCamera*> cameras() const;

    private:
        SceneCamera*    m_pDefaultCamera;
        OriginMarker*   m_pOriginMarker;
    };
}

#endif // MAPSCENE_H
