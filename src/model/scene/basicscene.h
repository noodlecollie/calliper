#ifndef BASICSCENE_H
#define BASICSCENE_H

#include "model_global.h"
#include "scene.h"

namespace NS_MODEL
{
    class SceneCamera;
    class OriginMarker;

    class MODELSHARED_EXPORT BasicScene : public Scene
    {
        Q_OBJECT
    public:
        explicit BasicScene(ShaderStore* shaderStore, TextureStore* textureStore, QObject* parent = 0);

        SceneCamera* defaultCamera() const;

    private:
        SceneCamera*    m_pDefaultCamera;
        OriginMarker*   m_pOriginMarker;
    };
}

#endif // BASICSCENE_H
