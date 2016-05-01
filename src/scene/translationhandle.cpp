#include "translationhandle.h"
#include "geometryfactory.h"
#include "resourcemanager.h"
#include "shaderstack.h"
#include "shaderprogram.h"
#include "openglrenderer.h"
#include "shaders.h"
#include <cmath>
#include <QtMath>
#include "callipermath.h"

#define PICKCOLOUR_X    0x88ff0000
#define PICKCOLOUR_Y    0x8800ff00
#define PICKCOLOUR_Z    0x880000ff
#define PICKCOLOUR_XY   0x88888800
#define PICKCOLOUR_YZ   0x88008888
#define PICKCOLOUR_XZ   0x88880088
#define PICKMASK        0x00ffffff

void addTranslationHead(float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
{
    static const float HEAD_RADIUS = 0.05f;
    static const float HEAD_LENGTH = 0.15f;

    GeometryData geometry;
    geometry.setShaderOverride(PerVertexColorShader::staticName());

    // Arrow head base circle
    for ( int i = 0; i < 8; i++ )
    {
        float radians = ((float)i * M_PI)/4.0f;
        QVector3D pos((1.0f - HEAD_LENGTH) * scale, HEAD_RADIUS * scale * qSin(radians), HEAD_RADIUS * scale * qCos(radians));
        geometry.appendVertex(pos, QVector3D(), col);
    }

    // Arrow head point
    int arrowPointIndex = geometry.vertexCount();
    geometry.appendVertex(QVector3D(scale,0,0), QVector3D(), col);

    // Arrow head circumference triangles
    for ( int i = 0; i < 8; i++ )
    {
        int j = i == 7 ? 0 : i+1;
        geometry.appendIndexTriangle(i, arrowPointIndex, j);
    }

    // Arrow head base triangles
    for ( int i = 1; i < 7; i++ )
    {
        geometry.appendIndexTriangle(0, i, i+1);
    }

    if ( !transform.isIdentity() )
    {
        geometry.transform(transform);
    }

    data.append(geometry);
}

void addTranslationPanel(float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
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

void addTranslationShaft(float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
{
    GeometryData geometry;
    geometry.setShaderOverride(PerVertexColorShader::staticName());

    // Arrow shaft (line)
    geometry.appendVertex(QVector3D(0,0,0), QVector3D(), col);
    geometry.appendVertex(QVector3D(0.85f * scale, 0, 0), QVector3D(), col);
    geometry.appendIndex(0);
    geometry.appendIndex(1);

    if ( !transform.isIdentity() )
    {
        geometry.transform(transform);
    }

    data.append(geometry);
}

void TranslationHandle::build()
{
    const float scale = 0.5f;
    GeometryData* arrows = new GeometryData();
    GeometryData* lines = new GeometryData();

    arrows->setDataFormat(GeometryData::PositionNormalColor);
    arrows->setShaderOverride(PerVertexColorShader::staticName());
    arrows->setDrawMode(GL_TRIANGLES);

    lines->setDataFormat(GeometryData::PositionNormalColor);
    lines->setShaderOverride(PerVertexColorShader::staticName());
    lines->setDrawMode(GL_LINES);

    addTranslationHead(scale, QColor::fromRgb(PICKCOLOUR_X), QMatrix4x4(), *arrows);
    addTranslationHead(scale, QColor::fromRgb(PICKCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *arrows);
    addTranslationHead(scale, QColor::fromRgb(PICKCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *arrows);

    addTranslationPanel(scale/4.0f, QColor::fromRgba(PICKCOLOUR_XY), QMatrix4x4(), *arrows);
    addTranslationPanel(scale/4.0f, QColor::fromRgba(PICKCOLOUR_YZ), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *arrows);
    addTranslationPanel(scale/4.0f, QColor::fromRgba(PICKCOLOUR_XZ), Math::matrixRotateX(qDegreesToRadians(90.0f)), *arrows);

    addTranslationShaft(scale, QColor::fromRgb(PICKCOLOUR_X), QMatrix4x4(), *lines);
    addTranslationShaft(scale, QColor::fromRgb(PICKCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *lines);
    addTranslationShaft(scale, QColor::fromRgb(PICKCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *lines);

    appendGeometry(arrows);
    appendGeometry(lines);
}

TranslationHandle::TranslationHandle(SceneObject *parent) : UIManipulator(parent)
{
    build();
}

TranslationHandle::TranslationHandle(const TranslationHandle &cloneFrom) : UIManipulator(cloneFrom)
{
    build();
}

TranslationHandle::~TranslationHandle()
{

}

void TranslationHandle::draw(ShaderStack *stack)
{
    UIManipulator::draw(stack);
}

SceneObject* TranslationHandle::clone() const
{
    return new TranslationHandle(*this);
}
