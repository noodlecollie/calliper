#ifndef SCENEFACTORY
#define SCENEFACTORY

class MapScene;
class UIScene;
class MapDocument;

namespace SceneFactory
{
    MapScene* defaultScene(MapDocument* document);
    MapScene* debugScene(MapDocument* document);

    UIScene* debugUIScene(MapDocument* document);
}

#endif // SCENEFACTORY

