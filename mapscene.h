#ifndef SCENE_H
#define SCENE_H

#include "basescene.h"
#include "iserialisable.h"

class MapDocument;
class MapGrid;

class MapScene : public BaseScene, public ISerialisable
{
    friend class MapGrid;
public:
    MapScene(MapDocument* document);
    virtual ~MapScene();
    virtual SceneType type() const;

    MapGrid* grid() const;

    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;
    bool unserialiseFromJson(const QJsonObject &serialisedData);

private:
    void insertStandardItems();
    bool serialiseRecursive(SceneObject* obj, QJsonObject &jsonParent) const;
    bool unserialiseRecursive(SceneObject* parent, const QJsonObject &serialisedData);

    MapGrid*       m_pGrid;
};

#endif // SCENE_H
