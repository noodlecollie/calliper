#ifndef RENDERMODELPASSATTRIBUTES_H
#define RENDERMODELPASSATTRIBUTES_H

#include "renderer_global.h"
#include <QMatrix4x4>

struct RENDERERSHARED_EXPORT RenderModelPassAttributes
{
public:
    RenderModelPassAttributes(const QMatrix4x4 &w2c, const QMatrix4x4 &projection);
    RenderModelPassAttributes();

    QMatrix4x4  worldToCameraMatrix;
    QMatrix4x4  projectionMatrix;

    int uniformDataSizeInBytes() const;
};

#endif // RENDERMODELPASSATTRIBUTES_H
