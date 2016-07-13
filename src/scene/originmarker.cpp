#include "originmarker.h"
#include "pervertexcolorshader.h"
#include "basescene.h"

OriginMarker::OriginMarker(BaseScene *scene, SceneObject *parent) : SceneObject(scene, parent)
{
    setUseCachedBounds(true);
    constructGeometry();
}

OriginMarker::~OriginMarker()
{
}

OriginMarker::OriginMarker(const OriginMarker &cloneFrom) : SceneObject(cloneFrom)
{

}

bool OriginMarker::editable() const
{
    return false;
}

void OriginMarker::constructGeometry()
{
    // TODO: Use multiple GeometryDatas now we have this capability!
    GeometryData* geom = new GeometryData();
    geom->setShaderOverride(PerVertexColorShader::staticName());

    geom->setDrawMode(GL_LINES);
    geom->appendVertex(QVector3D(0,0,0), QVector3D(), QColor(255,0,0));
    geom->appendVertex(QVector3D(64,0,0), QVector3D(), QColor(255,0,0));
    geom->appendVertex(QVector3D(0,0,0), QVector3D(), QColor(0,255,0));
    geom->appendVertex(QVector3D(0,64,0), QVector3D(), QColor(0,255,0));
    geom->appendVertex(QVector3D(0,0,0), QVector3D(), QColor(0,0,255));
    geom->appendVertex(QVector3D(0,0,64), QVector3D(), QColor(0,0,255));
    geom->appendIndex(0);
    geom->appendIndex(1);
    geom->appendIndex(2);
    geom->appendIndex(3);
    geom->appendIndex(4);
    geom->appendIndex(5);

    appendGeometry(geom);
}

BoundingBox OriginMarker::computeLocalBounds() const
{
    return BoundingBox(QVector3D(0,0,0), QVector3D(64,64,64));
}
