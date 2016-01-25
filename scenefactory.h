#ifndef SCENEFACTORY
#define SCENEFACTORY

class Scene;
class MapDocument;

namespace SceneFactory
{
    Scene* defaultScene(MapDocument* document);
    Scene* debugScene(MapDocument* document);

    Scene* debugUIScene(MapDocument* document);
}

#endif // SCENEFACTORY

