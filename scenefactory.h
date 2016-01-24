#ifndef SCENEFACTORY
#define SCENEFACTORY

class MapScene;
class MapDocument;

namespace SceneFactory
{
    MapScene* defaultScene(MapDocument* document);
    MapScene* debugScene(MapDocument* document);
}

#endif // SCENEFACTORY

