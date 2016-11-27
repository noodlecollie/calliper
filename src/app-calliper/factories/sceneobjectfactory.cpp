#include "sceneobjectfactory.h"
#include <functional>
#include "sceneobject.h"
#include "basescene.h"
#include "scenecamera.h"
#include "brush.h"

namespace SceneObjectFactory
{
    typedef std::function<SceneObject*(BaseScene*, const QJsonObject&, SceneObject*)> FactoryFunction;
    typedef QHash<QString, FactoryFunction> FunctionTable;

    FunctionTable functionTable;
    bool functionTableInitialised = false;

    void initialiseFunctionTable()
    {
        if ( functionTableInitialised )
            return;

        // SceneObject
        functionTable.insert(SceneObject::staticMetaObject.className(),
                             [](BaseScene* scene, const QJsonObject &serialisedData, SceneObject* parent) -> SceneObject*
        { return scene->createSceneObject<SceneObject>(serialisedData, parent); });

        // SceneCamera
        functionTable.insert(SceneCamera::staticMetaObject.className(),
                             [](BaseScene* scene, const QJsonObject &serialisedData, SceneObject* parent) -> SceneObject*
        { return scene->createSceneObject<SceneCamera>(serialisedData, parent); });

        // Brush
        functionTable.insert(Brush::staticMetaObject.className(),
                             [](BaseScene* scene, const QJsonObject &serialisedData, SceneObject* parent) -> SceneObject*
        { return scene->createSceneObject<Brush>(serialisedData, parent); });

        functionTableInitialised = true;
    }

    SceneObject* unserialiseFrom(BaseScene *scene, const QJsonObject &serialisedData, SceneObject *parent)
    {
        initialiseFunctionTable();
        static FactoryFunction defaultFunction = [] (BaseScene*, const QJsonObject&, SceneObject*) -> SceneObject*
        {
            return nullptr;
        };

        QString identifier = serialisedData.value(ISerialisable::KEY_IDENTIFIER()).toString();
        if ( identifier.isNull() )
            return nullptr;

        return functionTable.value(identifier, defaultFunction)(scene, serialisedData, parent);
    }
}
