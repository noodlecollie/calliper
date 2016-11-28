#ifndef VMF_H
#define VMF_H

#include "model-loaders_global.h"
#include "scene/sceneobject.h"

namespace ModelLoaders
{
    namespace VMF
    {
        MODELLOADERSSHARED_EXPORT void createBrushes(const QJsonDocument &doc, Model::SceneObject* parent);
    }
}

#endif // VMF_H
