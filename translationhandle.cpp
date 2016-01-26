#include "translationhandle.h"
#include "geometryfactory.h"
#include "resourcemanager.h"
#include "shaderstack.h"
#include "shaderprogram.h"
#include "openglrenderer.h"

TranslationHandle::TranslationHandle(SceneObject *parent) : SceneObject(parent)
{
    m_iShaftOffset = 0;
    setGeometry(GeometryFactory::translationHandle(64.0f, &m_iShaftOffset));
}

TranslationHandle::~TranslationHandle()
{

}

void TranslationHandle::draw(ShaderStack *stack)
{
    ShaderProgram* program = resourceManager()->shader(m_pGeometry->shaderOverride());
    Q_ASSERT(program);
    stack->shaderPush(program);

    stack->modelToWorldPostMultiply(localToParent());

    // Upload and bind the geometry.
    geometry()->upload();
    geometry()->bindVertices(true);
    geometry()->bindIndices(true);

    // Apply the data format.
    geometry()->applyDataFormat(stack->shaderTop());

    int firstBatch = m_iShaftOffset;
    int secondBatch = geometry()->indexCount() - m_iShaftOffset;

    // Draw.
    geometry()->setDrawMode(GL_TRIANGLES);
    geometry()->draw(0, firstBatch);

    geometry()->setDrawMode(GL_LINES);
    geometry()->draw(m_iShaftOffset * sizeof(unsigned int), secondBatch);

    stack->shaderPop();
}
