#ifndef VMF_H
#define VMF_H

#include "serialisation_global.h"
#include "scene/sceneobject.h"

namespace NS_SERIALISATION
{
    namespace VMF
    {
        void createBrushes(const QJsonDocument &doc, NS_MODEL::SceneObject* parent);
    }
}

#endif // VMF_H
