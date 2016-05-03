#ifndef SCENEOBJECTFACTORY_H
#define SCENEOBJECTFACTORY_H

#include <QString>
#include <QJsonObject>

class SceneObject;
class BaseScene;

namespace SceneObjectFactory
{
    typedef SceneObject* (*FactoryFuncPointer) (const QJsonObject&, BaseScene*, SceneObject*);
    FactoryFuncPointer getFactory(const QString &identifier);
}

#endif // SCENEOBJECTFACTORY_H
