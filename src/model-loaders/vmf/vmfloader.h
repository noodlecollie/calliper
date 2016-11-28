#ifndef VMFLOADER_H
#define VMFLOADER_H

#include "model-loaders_global.h"
#include "scene/sceneobject.h"

namespace ModelLoaders
{
    namespace VMFLoader
    {
        MODELLOADERSSHARED_EXPORT void createBrushes(const QJsonDocument &doc, Model::SceneObject* parent);
    }
}

#endif // VMFLOADER_H
