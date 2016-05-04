#ifndef SCENEOBJECTFACTORY_H
#define SCENEOBJECTFACTORY_H

#include <QJsonObject>

class SceneObject;
class BaseScene;

namespace SceneObjectFactory
{
    SceneObject* unserialiseFrom(BaseScene* scene, const QJsonObject &serialisedData, SceneObject* parent);
}

#endif // SCENEOBJECTFACTORY_H
