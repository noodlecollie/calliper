#include "scalehandle.h"
#include "geometryfactory.h"
#include "geometrydata.h"
#include "shaders.h"
#include <QtMath>

#define PICKCOLOUR_X    0x88ff0000
#define PICKCOLOUR_Y    0x8800ff00
#define PICKCOLOUR_Z    0x880000ff
#define PICKCOLOUR_XY   0x88888800
#define PICKCOLOUR_YZ   0x88008888
#define PICKCOLOUR_XZ   0x88880088
#define PICKMASK        0x00ffffff

void addScaleHead(float scale, float radius, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
{
    GeometryData* geom = GeometryFactory::cubeSolidColor(scale*radius, col, true);
    geom->transform(Math::matrixTranslate(QVector3D(scale - radius, 0, 0)));
    geom->transform(transform);
    data.append(*geom);
    delete geom;
}

void addScalePanel(float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
{
    GeometryData geometry;
    geometry.setShaderOverride(PerVertexColorShader::staticName());

    // Panel is in the XY plane.
    geometry.appendVertex(QVector3D(0,0,0), QVector3D(), col);
    geometry.appendVertex(QVector3D(scale,0,0), QVector3D(), col);
    geometry.appendVertex(QVector3D(scale,scale,0), QVector3D(), col);
    geometry.appendVertex(QVector3D(0,scale,0), QVector3D(), col);

    // Create two faces facing opposite directions.
    geometry.appendIndexTriangle(0,1,2);
    geometry.appendIndexTriangle(0,2,3);
    geometry.appendIndexTriangle(0,3,2);
    geometry.appendIndexTriangle(0,2,1);

    if ( !transform.isIdentity() )
    {
        geometry.transform(transform);
    }

    data.append(geometry);
}

void addScaleShaft(float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
{
    GeometryData geometry;
    geometry.setShaderOverride(PerVertexColorShader::staticName());

    // Arrow shaft (line)
    geometry.appendVertex(QVector3D(0,0,0), QVector3D(), col);
    geometry.appendVertex(QVector3D(scale, 0, 0), QVector3D(), col);
    geometry.appendIndex(0);
    geometry.appendIndex(1);

    if ( !transform.isIdentity() )
    {
        geometry.transform(transform);
    }

    data.append(geometry);
}

ScaleHandle::ScaleHandle(SceneObject *parent) : UIManipulator(parent)
{
    build();
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
    build();
}

void ScaleHandle::build()
{
    static const float SCALE = 0.5f;
    static const float HEAD_RADIUS = 0.15f/2.0f;

    GeometryData* heads = new GeometryData();
    GeometryData* lines = new GeometryData();

    heads->setDataFormat(GeometryData::PositionNormalColor);
    heads->setShaderOverride(PerVertexColorShader::staticName());
    heads->setDrawMode(GL_TRIANGLES);

    lines->setDataFormat(GeometryData::PositionNormalColor);
    lines->setShaderOverride(PerVertexColorShader::staticName());
    lines->setDrawMode(GL_LINES);

    addScaleHead(SCALE, HEAD_RADIUS, QColor::fromRgb(PICKCOLOUR_X), QMatrix4x4(), *heads);
    addScaleHead(SCALE, HEAD_RADIUS, QColor::fromRgb(PICKCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *heads);
    addScaleHead(SCALE, HEAD_RADIUS, QColor::fromRgb(PICKCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *heads);

    addScalePanel(SCALE/4.0f, QColor::fromRgba(PICKCOLOUR_XY), QMatrix4x4(), *heads);
    addScalePanel(SCALE/4.0f, QColor::fromRgba(PICKCOLOUR_YZ), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *heads);
    addScalePanel(SCALE/4.0f, QColor::fromRgba(PICKCOLOUR_XZ), Math::matrixRotateX(qDegreesToRadians(90.0f)), *heads);

    addScaleShaft(SCALE - HEAD_RADIUS, QColor::fromRgb(PICKCOLOUR_X), QMatrix4x4(), *lines);
    addScaleShaft(SCALE - HEAD_RADIUS, QColor::fromRgb(PICKCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *lines);
    addScaleShaft(SCALE - HEAD_RADIUS, QColor::fromRgb(PICKCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *lines);

    appendGeometry(heads);
    appendGeometry(lines);
}

void ScaleHandle::draw(ShaderStack *stack)
{
    UIManipulator::draw(stack);
}
