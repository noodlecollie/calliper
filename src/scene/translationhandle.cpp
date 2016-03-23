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

    //m_iXOffset = arrows->indexCount();
    addTranslationHead(scale, QColor::fromRgb(PICKCOLOUR_X), QMatrix4x4(), *arrows);
    //m_iAxisSectionLength = arrows->indexCount() - m_iXOffset;

    //m_iYOffset = geometry->indexCount();
    addTranslationHead(scale, QColor::fromRgb(PICKCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *arrows);

    //m_iZOffset = geometry->indexCount();
    addTranslationHead(scale, QColor::fromRgb(PICKCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *arrows);

    //m_iXYOffset = geometry->indexCount();
    addTranslationPanel(scale/4.0f, QColor::fromRgba(PICKCOLOUR_XY), QMatrix4x4(), *arrows);
    //m_iPlaneSectionLength = geometry->indexCount() - m_iXYOffset;

    //m_iYZOffset = geometry->indexCount();
    addTranslationPanel(scale/4, QColor::fromRgba(PICKCOLOUR_YZ), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *arrows);

    //m_iXZOffset = geometry->indexCount();
    addTranslationPanel(scale/4, QColor::fromRgba(PICKCOLOUR_XZ), Math::matrixRotateX(qDegreesToRadians(90.0f)), *arrows);

    //m_iShaftOffset = geometry->indexCount();

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
    m_iShaftOffset = cloneFrom.m_iShaftOffset;
    m_iXOffset = cloneFrom.m_iXOffset;
    m_iYOffset = cloneFrom.m_iYOffset;
    m_iZOffset = cloneFrom.m_iZOffset;
    m_iXYOffset = cloneFrom.m_iXYOffset;
    m_iYZOffset = cloneFrom.m_iYZOffset;
    m_iXZOffset = cloneFrom.m_iXZOffset;
    m_iAxisSectionLength = cloneFrom.m_iAxisSectionLength;
    m_iPlaneSectionLength = cloneFrom.m_iPlaneSectionLength;
}

TranslationHandle::~TranslationHandle()
{

}

void TranslationHandle::draw(ShaderStack *stack)
{
    QVector4D testVec = stack->cameraProjectionTop() * stack->coordinateTransformTop() * stack->worldToCameraTop()
            * stack->modelToWorldTop() * QVector4D(0,0,0,1);

    stack->counterScalePush();
    stack->counterScaleSetTop(testVec.z());

    for ( int i = 0; i < geometryCount(); i++ )
    {
        GeometryData* geom = geometryAt(i).data();

        ShaderProgram* program = resourceManager()->shader(geom->shaderOverride());
        Q_ASSERT(program);
        stack->shaderPush(program);

        // Upload and bind the geometry.
        geom->upload();
        geom->bindVertices(true);
        geom->bindIndices(true);

        // Apply the data format.
        geom->applyDataFormat(stack->shaderTop());

//        if ( stack->isPicking() )
//        {
//            drawForPicking(stack, geom);
//        }
//        else
//        {
            drawNormal(geom);
//        }

        stack->shaderPop();
    }

    stack->counterScalePop();
}

void TranslationHandle::drawNormal(GeometryData *geom)
{
//    int firstBatch = m_iShaftOffset;
//    int secondBatch = geom->indexCount() - m_iShaftOffset;

//    // Draw.
//    geom->setDrawMode(GL_TRIANGLES);
//    geom->draw(0, firstBatch);

//    geom->setDrawMode(GL_LINES);
//    geom->draw(m_iShaftOffset * sizeof(unsigned int), secondBatch);

    geom->draw();
}

void TranslationHandle::drawForPicking(ShaderStack *stack, GeometryData *geom)
{
    // Draw X.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_X);
    geom->setDrawMode(GL_TRIANGLES);
    geom->draw(m_iXOffset * sizeof(unsigned int), m_iAxisSectionLength);
    geom->setDrawMode(GL_LINES);
    geom->draw((m_iShaftOffset) * sizeof(unsigned int), 2);
    stack->globalColorPop();

    // Draw Y.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_Y);
    geom->setDrawMode(GL_TRIANGLES);
    geom->draw(m_iYOffset * sizeof(unsigned int), m_iAxisSectionLength);
    geom->setDrawMode(GL_LINES);
    geom->draw((m_iShaftOffset+2) * sizeof(unsigned int), 2);
    stack->globalColorPop();

    // Draw Z.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_Z);
    geom->setDrawMode(GL_TRIANGLES);
    geom->draw(m_iZOffset * sizeof(unsigned int), m_iAxisSectionLength);
    geom->setDrawMode(GL_LINES);
    geom->draw((m_iShaftOffset+4) * sizeof(unsigned int), 2);
    stack->globalColorPop();

    geom->setDrawMode(GL_TRIANGLES);

    // Draw XY.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_XY);
    geom->draw(m_iXYOffset * sizeof(unsigned int), m_iPlaneSectionLength);
    stack->globalColorPop();

    // Draw XZ.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_XZ);
    geom->draw(m_iXZOffset * sizeof(unsigned int), m_iPlaneSectionLength);
    stack->globalColorPop();

    // Draw YZ.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_YZ);
    geom->draw(m_iYZOffset * sizeof(unsigned int), m_iPlaneSectionLength);
    stack->globalColorPop();
}

int TranslationHandle::axisFlagsFromPickColor(QRgb colour)
{
    switch (colour & PICKMASK)
    {
        case PICKCOLOUR_X & PICKMASK:
            return UIManipulator::AxisX;

        case PICKCOLOUR_Y & PICKMASK:
            return UIManipulator::AxisY;

        case PICKCOLOUR_Z & PICKMASK:
            return UIManipulator::AxisZ;

        case PICKCOLOUR_XY & PICKMASK:
            return UIManipulator::AxisXY;

        case PICKCOLOUR_XZ & PICKMASK:
            return UIManipulator::AxisXZ;

        case PICKCOLOUR_YZ & PICKMASK:
            return UIManipulator::AxisYZ;

        default:
            return 0;
    }
}

SceneObject* TranslationHandle::clone() const
{
    return new TranslationHandle(*this);
}
