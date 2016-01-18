#include "translationhandle.h"
#include "geometrydata.h"
#include "geometryfactory.h"
#include "callipermath.h"
#include <QtMath>
#include "shaderstack.h"
#include "resourcemanager.h"
#include "shaders.h"

TranslationHandle::TranslationHandle(SceneObject *parent) : SceneObject(parent)
{
    buildGeometry();
    setIgnoreDepth(true);
}

TranslationHandle::~TranslationHandle()
{

}

void TranslationHandle::buildGeometry()
{
    setGeometry(GeometryFactory::translationHandle(64.0f, QColor::fromRgb(0xffff0000)));
    m_iSegmentSize = m_pGeometry->indexCount();

    {
        GeometryData* data = GeometryFactory::translationHandle(64.0f, QColor::fromRgb(0xff00ff00),
                                                                Math::matrixRotateZ(qDegreesToRadians(90.0f)));
        m_pGeometry->append(*data);
        delete data;
    }

    {
        GeometryData* data = GeometryFactory::translationHandle(64.0f, QColor::fromRgb(0xff0000ff),
                                                                Math::matrixRotateY(qDegreesToRadians(-90.0f)));
        m_pGeometry->append(*data);
        delete data;
    }

    m_pGeometry->setLineWidth(2);
}

void TranslationHandle::draw(ShaderStack *stack)
{
    stack->shaderPush(resourceManager()->shader(PerVertexColorShader::staticName()));
    stack->modelToWorldPostMultiply(localToParent());

    m_pGeometry->upload();
    m_pGeometry->bindVertices(true);
    m_pGeometry->bindIndices(true);
    m_pGeometry->applyDataFormat(stack->shaderTop());

    for ( int i = 0; i < 3; i++ )
    {
        int base = i * m_iSegmentSize * sizeof(unsigned int);

        // Draw the arrow head.
        m_pGeometry->setDrawMode(GL_TRIANGLES);
        m_pGeometry->draw(base, m_iSegmentSize-2);

        // Draw the arrow shaft.
        m_pGeometry->setDrawMode(GL_LINES);
        m_pGeometry->draw(base + ((m_iSegmentSize - 2) * sizeof(unsigned int)), 2);
    }

    stack->shaderPop();
}
