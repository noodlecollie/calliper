#include "sceneobjectfactory.h"
#include "sceneobject.h"
#include "scenecamera.h"
#include "basescene.h"

namespace FactoryFunctions
{
    SceneObject* createSceneObject(const QJsonObject &serialisedData, BaseScene* scene, SceneObject* parent)
    {
        return scene->createSceneObject<SceneObject>(serialisedData, parent);
    }

    SceneObject* createSceneCamera(const QJsonObject &serialisedData, BaseScene* scene, SceneObject* parent)
    {
        return scene->createSceneObject<SceneCamera>(serialisedData, parent);
    }
}

namespace SceneObjectFactory
{
    typedef QHash<QString, FactoryFuncPointer> FunctionTable;

    FunctionTable functionTable;
    bool functionTableInitialised = false;

    void initialiseFunctionTable()
    {
        if ( functionTableInitialised )
            return;

        functionTable.insert(SceneObject::staticMetaObject.className(), &FactoryFunctions::createSceneObject);
        functionTable.insert(SceneCamera::staticMetaObject.className(), &FactoryFunctions::createSceneCamera);

        functionTableInitialised = true;
    }

    FactoryFuncPointer getFactory(const QString &identifier)
    {
        initialiseFunctionTable();

        FactoryFuncPointer fPointer = functionTable.value(identifier, NULL);
        if ( !fPointer )
        {
            qWarning() << "WARNING: No compatible factory function present for SceneObject type" << identifier;
        }

        return fPointer;
    }
}
