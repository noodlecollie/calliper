#ifndef SCENEFACTORY
#define SCENEFACTORY

class MapScene;
class UIScene;
class MapDocument;
class QJsonDocument;

namespace SceneFactory
{
    MapScene* defaultScene(MapDocument* document);
    MapScene* debugScene(MapDocument* document);

    UIScene* defaultUIScene(MapDocument* document);
    UIScene* debugUIScene(MapDocument* document);

    void populateFromVMF(MapScene* scene, const QJsonDocument &vmf);
}

#endif // SCENEFACTORY

