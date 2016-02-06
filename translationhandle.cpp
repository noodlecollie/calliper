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

#define PICKCOLOUR_X 0xffff0000
#define PICKCOLOUR_Y 0xff00ff00
#define PICKCOLOUR_Z 0xff0000ff
#define PICKCOLOUR_XY 0xff888800
#define PICKCOLOUR_YZ 0xff008888
#define PICKCOLOUR_XZ 0xff880088

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
        geometry.appendVertex(QVector3D((1.0f - HEAD_LENGTH) * scale,
                                         HEAD_RADIUS * scale * qSin(radians),
                                         HEAD_RADIUS * scale * qCos(radians)),
                               col);
    }

    // Arrow head point
    int arrowPointIndex = geometry.vertexCount();
    geometry.appendVertex(QVector3D(scale,0,0), col);

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
    geometry.appendVertex(QVector3D(0,0,0), col);
    geometry.appendVertex(QVector3D(scale,0,0), col);
    geometry.appendVertex(QVector3D(scale,scale,0), col);
    geometry.appendVertex(QVector3D(0,scale,0), col);

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
    geometry.appendVertex(QVector3D(0,0,0),
                           col);
    geometry.appendVertex(QVector3D(0.85f * scale, 0, 0),
                           col);
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
    GeometryData* geometry = new GeometryData;
    geometry->setDataFormat(GeometryData::PositionColor);
    geometry->setShaderOverride(PerVertexColorShader::staticName());
    geometry->setDrawMode(GL_TRIANGLES);

    m_iXOffset = geometry->indexCount();
    addTranslationHead(scale, QColor::fromRgb(0xffff0000), QMatrix4x4(), *geometry);
    m_iAxisSectionLength = geometry->indexCount() - m_iXOffset;

    m_iYOffset = geometry->indexCount();
    addTranslationHead(scale, QColor::fromRgb(0xff00ff00), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *geometry);

    m_iZOffset = geometry->indexCount();
    addTranslationHead(scale, QColor::fromRgb(0xff0000ff), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *geometry);

    m_iXYOffset = geometry->indexCount();
    addTranslationPanel(scale/4.0f, QColor::fromRgba(0x88ffff00), QMatrix4x4(), *geometry);
    m_iPlaneSectionLength = geometry->indexCount() - m_iXYOffset;

    m_iYZOffset = geometry->indexCount();
    addTranslationPanel(scale/4, QColor::fromRgba(0x8800ffff), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *geometry);

    m_iXZOffset = geometry->indexCount();
    addTranslationPanel(scale/4, QColor::fromRgba(0x88ff00ff), Math::matrixRotateX(qDegreesToRadians(90.0f)), *geometry);

    m_iShaftOffset = geometry->indexCount();

    addTranslationShaft(scale, QColor::fromRgb(0xffff0000), QMatrix4x4(), *geometry);
    addTranslationShaft(scale, QColor::fromRgb(0xff00ff00), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *geometry);
    addTranslationShaft(scale, QColor::fromRgb(0xff0000ff), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *geometry);

    setGeometry(geometry);
}

TranslationHandle::TranslationHandle(SceneObject *parent) : UIManipulator(parent)
{
    build();
}

TranslationHandle::~TranslationHandle()
{

}

void TranslationHandle::draw(ShaderStack *stack)
{
//    ShaderProgram* program = resourceManager()->shader(m_pGeometry->shaderOverride());
//    Q_ASSERT(program);
//    stack->shaderPush(program);
    stack->shaderPush(resourceManager()->shader(SelectionMaskShader::staticName()));

    stack->modelToWorldPostMultiply(localToParent());

    QVector4D testVec = stack->cameraProjectionTop() * stack->coordinateTransformTop() * stack->worldToCameraTop()
            * stack->modelToWorldTop() * QVector4D(0,0,0,1);

    stack->counterScalePush();
    stack->counterScaleSetTop(testVec.z());

    // Upload and bind the geometry.
    geometry()->upload();
    geometry()->bindVertices(true);
    geometry()->bindIndices(true);

    // Apply the data format.
    geometry()->applyDataFormat(stack->shaderTop());

    if ( stack->isPicking() )
    {
        drawForPicking(stack);
    }
    else
    {
        //drawNormal();
        drawForPicking(stack);
    }

    stack->counterScalePop();
    stack->shaderPop();
}

void TranslationHandle::drawNormal()
{
    int firstBatch = m_iShaftOffset;
    int secondBatch = geometry()->indexCount() - m_iShaftOffset;

    // Draw.
    geometry()->setDrawMode(GL_TRIANGLES);
    geometry()->draw(0, firstBatch);

    geometry()->setDrawMode(GL_LINES);
    geometry()->draw(m_iShaftOffset * sizeof(unsigned int), secondBatch);
}

void TranslationHandle::drawForPicking(ShaderStack *stack)
{
    // Draw X.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_X);
    geometry()->setDrawMode(GL_TRIANGLES);
    geometry()->draw(m_iXOffset * sizeof(unsigned int), m_iAxisSectionLength);
    geometry()->setDrawMode(GL_LINES);
    geometry()->draw((m_iShaftOffset) * sizeof(unsigned int), 2);
    stack->globalColorPop();

    // Draw Y.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_Y);
    geometry()->setDrawMode(GL_TRIANGLES);
    geometry()->draw(m_iYOffset * sizeof(unsigned int), m_iAxisSectionLength);
    geometry()->setDrawMode(GL_LINES);
    geometry()->draw((m_iShaftOffset+2) * sizeof(unsigned int), 2);
    stack->globalColorPop();

    // Draw Z.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_Z);
    geometry()->setDrawMode(GL_TRIANGLES);
    geometry()->draw(m_iZOffset * sizeof(unsigned int), m_iAxisSectionLength);
    geometry()->setDrawMode(GL_LINES);
    geometry()->draw((m_iShaftOffset+4) * sizeof(unsigned int), 2);
    stack->globalColorPop();

    geometry()->setDrawMode(GL_TRIANGLES);

    // Draw XY.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_XY);
    geometry()->draw(m_iXYOffset * sizeof(unsigned int), m_iPlaneSectionLength);
    stack->globalColorPop();

    // Draw XZ.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_XZ);
    geometry()->draw(m_iXZOffset * sizeof(unsigned int), m_iPlaneSectionLength);
    stack->globalColorPop();

    // Draw YZ.
    stack->globalColorPush();
    stack->globalColorSetTop(PICKCOLOUR_YZ);
    geometry()->draw(m_iYZOffset * sizeof(unsigned int), m_iPlaneSectionLength);
    stack->globalColorPop();
}

int TranslationHandle::axisFlagsFromPickColor(QRgb colour)
{
    switch (colour)
    {
        case PICKCOLOUR_X:
            return UIManipulator::AxisX;

        case PICKCOLOUR_Y:
            return UIManipulator::AxisY;

        case PICKCOLOUR_Z:
            return UIManipulator::AxisZ;

        case PICKCOLOUR_XY:
            return UIManipulator::AxisXY;

        case PICKCOLOUR_XZ:
            return UIManipulator::AxisXZ;

        case PICKCOLOUR_YZ:
            return UIManipulator::AxisYZ;

        default:
            return 0;
    }
}
