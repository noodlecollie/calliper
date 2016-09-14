#include "rendermodelpassattributes.h"

RenderModelPassAttributes::RenderModelPassAttributes(const QMatrix4x4 &w2c, const QMatrix4x4 &projection)
    : worldToCameraMatrix(w2c), projectionMatrix(projection)
{

}

RenderModelPassAttributes::RenderModelPassAttributes()
{

}

int RenderModelPassAttributes::uniformDataSizeInBytes() const
{
    return 2 * 16 * sizeof(float);
}
