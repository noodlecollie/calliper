#include "sceneobjectfactory.h"
#include "sceneobject.h"
#include <QHash>
#include <QtDebug>
#include "scenecamera.h"

namespace FactoryFunctions
{
    SceneObject* createSceneObject(const QJsonObject &serialisedData, SceneObject* parent)
    {
        return new SceneObject(serialisedData, parent);
    }

    SceneObject* createSceneCamera(const QJsonObject &serialisedData, SceneObject* parent)
    {
        return new SceneCamera(serialisedData, parent);
    }
}

namespace SceneObjectFactory
{
    typedef SceneObject* (*FactoryFuncPointer) (const QJsonObject&, SceneObject*);
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

    SceneObject* unserialiseFromJson(const QJsonObject &serialisedData, SceneObject* parent)
    {
        initialiseFunctionTable();

        QString identifier = serialisedData.value(ISerialisable::KEY_IDENTIFIER()).toString();
        if ( identifier.isNull() )
            return NULL;

        FactoryFuncPointer fPointer = functionTable.value(identifier, NULL);
        if ( !fPointer )
        {
            qWarning() << "WARNING: No compatible factory function present for SceneObject type" << identifier;
            return NULL;
        }

        return fPointer(serialisedData, parent);
    }
}
