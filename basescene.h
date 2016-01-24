#ifndef BASESCENE_H
#define BASESCENE_H

#include <QList>

class SceneObject;
class SceneCamera;

class BaseScene
{
public:
    enum SceneType
    {
        TypeBase = 0,
        TypeMap,
        TypeUI,
    };

    BaseScene();
    virtual ~BaseScene();
    virtual SceneType sceneType() const;

    SceneObject* root() const;

    QList<SceneCamera*> findCameras() const;
    QList<SceneObject*> findByName(const QString &name);

protected:
    SceneObject*    m_pRootObject;
};

#endif // BASESCENE_H
