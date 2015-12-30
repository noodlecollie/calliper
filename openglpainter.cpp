#include "openglpainter.h"
#include "shaderprogram.h"

OpenGLPainter::OpenGLPainter(ShaderProgram* initial, bool autoUpdate)
{
    m_bAutoUpdate = autoUpdate;
    m_Shaders.push(initial);
    m_ModelToWorld.push(QMatrix4x4());
    m_WorldToCamera.push(QMatrix4x4());
    m_CoordinateTransform.push(QMatrix4x4());
    m_CameraProjection.push(QMatrix4x4());

    applyAll();
}

bool OpenGLPainter::autoUpdate() const
{
    return m_bAutoUpdate;
}

void OpenGLPainter::setAutoUpdate(bool enabled)
{
    m_bAutoUpdate = enabled;
}

// Should be called whenever a new shader is applied.
void OpenGLPainter::applyAll()
{
    ShaderProgram* p = m_Shaders.top();
    p->setUniformMatrix4(ShaderProgram::ModelToWorldMatrix, m_ModelToWorld.top());
    p->setUniformMatrix4(ShaderProgram::WorldToCameraMatrix, m_WorldToCamera.top());
    p->setUniformMatrix4(ShaderProgram::CoordinateTransformMatrix, m_CoordinateTransform.top());
    p->setUniformMatrix4(ShaderProgram::CameraProjectionMatrix, m_CameraProjection.top());
}

void OpenGLPainter::shaderPush(ShaderProgram *program)
{
    m_Shaders.top()->release();
    m_Shaders.push(program);
    m_Shaders.top()->apply();
    if ( m_bAutoUpdate )
        applyAll();
}

void OpenGLPainter::shaderPop()
{
    m_Shaders.top()->release();
    m_Shaders.pop();
    Q_ASSERT(m_Shaders.count() >= 1);
    m_Shaders.top()->apply();
    if ( m_bAutoUpdate )
        applyAll();
}

ShaderProgram* OpenGLPainter::shaderTop() const
{
    return m_Shaders.top();
}

int OpenGLPainter::shaderCount() const
{
    return m_Shaders.count();
}

void OpenGLPainter::modelToWorldPostMultiply(const QMatrix4x4 &mat)
{
    postMultiplyTop(m_ModelToWorld, ShaderProgram::ModelToWorldMatrix, mat);
}

void OpenGLPainter::modelToWorldPush()
{
    m_ModelToWorld.push(m_ModelToWorld.top());
}

void OpenGLPainter::modelToWorldPop()
{
    pop(m_ModelToWorld, ShaderProgram::ModelToWorldMatrix);
}

void OpenGLPainter::modelToWorldPreMultiply(const QMatrix4x4 &mat)
{
    preMultiplyTop(m_ModelToWorld, ShaderProgram::ModelToWorldMatrix, mat);
}

const QMatrix4x4& OpenGLPainter::modelToWorldTop() const
{
    return m_ModelToWorld.top();
}

int OpenGLPainter::modelToWorldCount() const
{
    return m_ModelToWorld.count();
}

void OpenGLPainter::preMultiplyTop(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att, const QMatrix4x4 &mat)
{
    stack.top() = mat * stack.top();
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformMatrix4(att, stack.top());
}

void OpenGLPainter::postMultiplyTop(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att, const QMatrix4x4 &mat)
{
    stack.top() = stack.top() * mat;
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformMatrix4(att, stack.top());
}

void OpenGLPainter::pop(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att)
{
    stack.pop();
    Q_ASSERT(stack.count() >= 1);
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformMatrix4(att, stack.top());
}

void OpenGLPainter::worldToCameraPreMultiply(const QMatrix4x4 &mat)
{
    preMultiplyTop(m_WorldToCamera, ShaderProgram::WorldToCameraMatrix, mat);
}

void OpenGLPainter::worldToCameraPostMultiply(const QMatrix4x4 &mat)
{
    postMultiplyTop(m_WorldToCamera, ShaderProgram::WorldToCameraMatrix, mat);
}

void OpenGLPainter::worldToCameraPush()
{
    m_WorldToCamera.push(m_WorldToCamera.top());
}

void OpenGLPainter::worldToCameraPop()
{
    pop(m_WorldToCamera, ShaderProgram::WorldToCameraMatrix);
}

const QMatrix4x4& OpenGLPainter::worldToCameraTop() const
{
    return m_WorldToCamera.top();
}

int OpenGLPainter::worldToCameraCount() const
{
    return m_WorldToCamera.count();
}

void OpenGLPainter::coordinateTransformPreMultiply(const QMatrix4x4 &mat)
{
    preMultiplyTop(m_CoordinateTransform, ShaderProgram::CoordinateTransformMatrix, mat);
}

void OpenGLPainter::coordinateTransformPostMultiply(const QMatrix4x4 &mat)
{
    postMultiplyTop(m_CoordinateTransform, ShaderProgram::CoordinateTransformMatrix, mat);
}

void OpenGLPainter::coordinateTransformPush()
{
    m_CoordinateTransform.push(m_CoordinateTransform.top());
}

void OpenGLPainter::coordinateTransformPop()
{
    pop(m_CoordinateTransform, ShaderProgram::CoordinateTransformMatrix);
}

const QMatrix4x4& OpenGLPainter::coordinateTransformTop() const
{
    return m_CoordinateTransform.top();
}

int OpenGLPainter::coordinateTransformCount() const
{
    return m_CoordinateTransform.count();
}

void OpenGLPainter::cameraProjectionPreMultiply(const QMatrix4x4 &mat)
{
    preMultiplyTop(m_CameraProjection, ShaderProgram::CameraProjectionMatrix, mat);
}

void OpenGLPainter::cameraProjectionPostMultiply(const QMatrix4x4 &mat)
{
    postMultiplyTop(m_CameraProjection, ShaderProgram::CameraProjectionMatrix, mat);
}

void OpenGLPainter::cameraProjectionPush()
{
    m_CameraProjection.push(m_CameraProjection.top());
}

void OpenGLPainter::cameraProjectionPop()
{
    pop(m_CameraProjection, ShaderProgram::CameraProjectionMatrix);
}

const QMatrix4x4& OpenGLPainter::cameraProjectionTop() const
{
    return m_CameraProjection.top();
}

int OpenGLPainter::cameraProjectionCount() const
{
    return m_CameraProjection.count();
}
