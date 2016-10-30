#ifndef GENERICBRUSHFACTORY_H
#define GENERICBRUSHFACTORY_H

#include "model_global.h"
#include "genericbrush/genericbrush.h"
#include "math/texturedwinding.h"
#include <QVector>

namespace NS_MODEL
{
    namespace GenericBrushFactory
    {
        MODELSHARED_EXPORT GenericBrush* createBrushFromWindingGroup(SceneObject* parent, QList<TexturedWinding*>& windings);
    }
}

#endif // GENERICBRUSHFACTORY_H
