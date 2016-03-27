#include "scalehandle.h"
#include "geometryfactory.h"
#include "geometrydata.h"

void addScaleHead(float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
{
    GeometryData* geom = GeometryFactory::cubeSolidColor(scale, col);
    geom->transform(Math::matrixTranslate(QVector3D(1 - (2*scale), 0, 0)));
    geom->transform(transform);
    data.append(*geom);
    delete geom;
}

ScaleHandle::ScaleHandle(SceneObject *parent) : UIManipulator(parent)
{

}

ScaleHandle::~ScaleHandle()
{

}

SceneObject* ScaleHandle::clone() const
{
    return new ScaleHandle(*this);
}

ScaleHandle::ScaleHandle(const ScaleHandle &cloneFrom) : UIManipulator(cloneFrom)
{

}
