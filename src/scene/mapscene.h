#ifndef MAPSCENE_H
#define MAPSCENE_H

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

protected:
    virtual void sceneClearedEvent();

private:
    void insertStandardItems();
    bool serialiseRecursive(SceneObject* obj, QJsonObject &jsonParent) const;
    bool unserialiseRecursive(SceneObject* parent, const QJsonObject &serialisedData);

    MapGrid*       m_pGrid;
};

#endif // MAPSCENE_H
