#ifndef SCENEOBJECTFACTORY_H
#define SCENEOBJECTFACTORY_H

#include <QJsonObject>

class SceneObject;

namespace SceneObjectFactory
{
    SceneObject* unserialiseFromJson(const QJsonObject &serialisedData, SceneObject* parent);
}

#endif // SCENEOBJECTFACTORY_H
