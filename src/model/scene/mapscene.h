#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "model_global.h"
#include "scene/basescene.h"
#include "serialisation/iserialisable.h"

namespace NS_MODEL
{
    class MapDocument;
    class MapGrid;

    class MODELSHARED_EXPORT MapScene : public BaseScene, public ISerialisable
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

    private slots:
        void checkIfSceneCleared(SceneObject* obj);

    private:
        void insertStandardItems();
        bool serialiseRecursive(SceneObject* obj, QJsonObject &jsonParent) const;
        bool unserialiseRecursive(SceneObject* parent, const QJsonObject &serialisedData);

        MapGrid*       m_pGrid;
    };
}

#endif // MAPSCENE_H
