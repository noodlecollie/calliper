#ifndef GENERICBRUSHFACTORY_H
#define GENERICBRUSHFACTORY_H

#include "model_global.h"
#include "genericbrush/genericbrush.h"
#include "math/texturedwinding.h"

namespace NS_MODEL
{
    namespace GenericBrushFactory
    {
        GenericBrush* createBrushFromWindingGroup(SceneObject* parent, const QList<TexturedWinding*> windings);
    }
}

#endif // GENERICBRUSHFACTORY_H
