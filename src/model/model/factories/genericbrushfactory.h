#ifndef GENERICBRUSHFACTORY_H
#define GENERICBRUSHFACTORY_H

#include "model_global.h"
#include "model/genericbrush/genericbrush.h"
#include "model/math/texturedwinding.h"
#include <QVector>

namespace Model
{
    namespace GenericBrushFactory
    {
        MODELSHARED_EXPORT GenericBrush* createBrushFromWindingGroup(SceneObject* parent, QList<TexturedWinding*>& windings);
        MODELSHARED_EXPORT GenericBrush* createBrushFromMinMaxVectors(SceneObject* parent, const QVector3D& min, const QVector3D& max);
    }
}

#endif // GENERICBRUSHFACTORY_H
