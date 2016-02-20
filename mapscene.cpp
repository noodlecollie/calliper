#include "mapscene.h"
#include "scenecamera.h"
#include "sceneobject.h"
#include "mapgrid.h"
#include <QJsonArray>

MapScene::MapScene(MapDocument *document) : BaseScene(document)
{
    m_pGrid = NULL;
}

MapScene::~MapScene()
{
}

BaseScene::SceneType MapScene::type() const
{
    return TypeMapScene;
}

MapGrid* MapScene::grid() const
{
    return m_pGrid;
}

bool MapScene::serialiseToJson(QJsonObject &obj) const
{
    obj.insert(ISerialisable::KEY_IDENTIFIER(), QJsonValue(serialiseIdentifier()));

    QJsonObject rootObj;
    serialiseRecursive(m_pRootObject, rootObj);
    obj.insert("root", QJsonValue(rootObj));

    return true;
}

QString MapScene::serialiseIdentifier() const
{
    return "MapScene";
}

bool MapScene::serialiseRecursive(SceneObject *obj, QJsonObject &jsonParent) const
{
    // Serialise this object.
    if ( !obj->serialiseToJson(jsonParent) )
        return false;

    // If this object has children, serialise them too.
    QList<SceneObject*> childList = obj->children();
    if ( childList.count() > 0 )
    {
        // Create an array to store the children in.
        QJsonArray arrChildren;

        // Serialise each child.
        foreach ( SceneObject* child, childList )
        {
            QJsonObject childObj;
            if ( serialiseRecursive(child, childObj) )
            {
                arrChildren.append(QJsonValue(childObj));
            }
        }

        // Add this array to the JSON parent.
        jsonParent.insert("children", QJsonValue(arrChildren));
    }

    return true;
}
