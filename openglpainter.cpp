#include "openglpainter.h"
#include "shaderprogram.h"

OpenGLPainter::OpenGLPainter(ShaderProgram* initial, bool autoUpdate)
{
    m_bAutoUpdate = autoUpdate;
    m_pCamera = NULL;
    m_Shaders.push(initial);
    m_Shaders.top()->apply();

    m_ModelToWorld.push(QMatrix4x4());
    m_WorldToCamera.push(QMatrix4x4());
    m_CoordinateTransform.push(QMatrix4x4());
    m_CameraProjection.push(QMatrix4x4());
    m_FogColor.push(QColor::fromRgb(0xffffffff));
    m_FogBegin.push(0);
    m_FogEnd.push(0);
    m_DirectionalLight.push(QVector3D(1,0,0));
    m_GlobalColor.push(QColor::fromRgb(0xffffffff));

    if ( m_bAutoUpdate )
        applyAll();
}

OpenGLPainter::~OpenGLPainter()
{
    ShaderProgram* p = m_Shaders.top();
    if ( p )
        p->release();
}

const Camera* OpenGLPainter::camera() const
{
    return m_pCamera;
}

void OpenGLPainter::setCamera(const Camera *camera)
{
    m_pCamera = camera;
}

bool OpenGLPainter::autoUpdate() const
{
    return m_bAutoUpdate;
}

void OpenGLPainter::setAutoUpdate(bool enabled)
{
    m_bAutoUpdate = enabled;
}

bool OpenGLPainter::inInitialState() const
{
    return m_Shaders.count() == 1 &&
            m_ModelToWorld.count() == 1 &&
            m_WorldToCamera.count() == 1 &&
            m_CoordinateTransform.count() == 1 &&
            m_CameraProjection.count() == 1 &&
            m_FogColor.count() == 1 &&
            m_FogBegin.count() == 1 &&
            m_FogEnd.count() == 1 &&
            m_DirectionalLight.count() == 1 &&
            m_GlobalColor.count() == 1;
}

// Should be called whenever a new shader is applied.
void OpenGLPainter::applyAll()
{
    ShaderProgram* p = m_Shaders.top();
    p->setUniformMatrix4(ShaderProgram::ModelToWorldMatrix, m_ModelToWorld.top());
    p->setUniformMatrix4(ShaderProgram::WorldToCameraMatrix, m_WorldToCamera.top());
    p->setUniformMatrix4(ShaderProgram::CoordinateTransformMatrix, m_CoordinateTransform.top());
    p->setUniformMatrix4(ShaderProgram::CameraProjectionMatrix, m_CameraProjection.top());
    p->setUniformColor4(ShaderProgram::FogColorUniform, m_FogColor.top());
    p->setUniformFloat(ShaderProgram::FogBeginUniform, m_FogBegin.top());
    p->setUniformFloat(ShaderProgram::FogEndUniform, m_FogEnd.top());
    p->setUniformVector3(ShaderProgram::DirectionalLightUniform, m_DirectionalLight.top());
    p->setUniformColor4(ShaderProgram::ColorUniform, m_GlobalColor.top());
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

void OpenGLPainter::pop(QStack<QColor> &stack, ShaderProgram::Attribute att)
{
    stack.pop();
    Q_ASSERT(stack.count() >= 1);
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformColor4(att, stack.top());
}

void OpenGLPainter::setTop(QStack<QColor> &stack, ShaderProgram::Attribute att, const QColor &col)
{
    if ( col == stack.top() ) return;

    stack.top() = col;
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformColor4(att, stack.top());
}

void OpenGLPainter::fogColorSetTop(const QColor &col)
{
    setTop(m_FogColor, ShaderProgram::FogColorUniform, col);
}

void OpenGLPainter::fogColorPush()
{
    m_FogColor.push(m_FogColor.top());
}

void OpenGLPainter::fogColorPop()
{
    pop(m_FogColor, ShaderProgram::FogColorUniform);
}
const QColor& OpenGLPainter::fogColorTop() const
{
    return m_FogColor.top();
}

int OpenGLPainter::fogColorCount() const
{
    return m_FogColor.count();
}

void OpenGLPainter::setTop(QStack<float> &stack, ShaderProgram::Attribute att, float value)
{
    if ( value == stack.top() ) return;

    stack.top() = value;
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformFloat(att, stack.top());
}

void OpenGLPainter::pop(QStack<float> &stack, ShaderProgram::Attribute att)
{
    stack.pop();
    Q_ASSERT(stack.count() >= 1);
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformFloat(att, stack.top());
}

void OpenGLPainter::fogBeginSetTop(float val)
{
    setTop(m_FogBegin, ShaderProgram::FogBeginUniform, val);
}

void OpenGLPainter::fogBeginPush()
{
    m_FogBegin.push(m_FogBegin.top());
}

void OpenGLPainter::fogBeginPop()
{
    pop(m_FogBegin, ShaderProgram::FogBeginUniform);
}

const float& OpenGLPainter::fogBeginTop() const
{
    return m_FogBegin.top();
}

int OpenGLPainter::fogBeginCount() const
{
    return m_FogBegin.count();
}

void OpenGLPainter::fogEndSetTop(float val)
{
    setTop(m_FogEnd, ShaderProgram::FogEndUniform, val);
}

void OpenGLPainter::fogEndPush()
{
    m_FogEnd.push(m_FogEnd.top());
}

void OpenGLPainter::fogEndPop()
{
    pop(m_FogEnd, ShaderProgram::FogEndUniform);
}

const float& OpenGLPainter::fogEndTop() const
{
    return m_FogEnd.top();
}

int OpenGLPainter::fogEndCount() const
{
    return m_FogEnd.count();
}

void OpenGLPainter::setTop(QStack<QVector3D> &stack, ShaderProgram::Attribute att, const QVector3D &value)
{
    if ( value == stack.top() ) return;

    stack.top() = value;
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformVector3(att, stack.top());
}

void OpenGLPainter::pop(QStack<QVector3D> &stack, ShaderProgram::Attribute att)
{
    stack.pop();
    Q_ASSERT(stack.count() >= 1);
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformVector3(att, stack.top());
}

void OpenGLPainter::directionalLightSetTop(const QVector3D &val)
{
    setTop(m_DirectionalLight, ShaderProgram::DirectionalLightUniform, val);
}

void OpenGLPainter::directionalLightPush()
{
    m_DirectionalLight.push(m_DirectionalLight.top());
}

void OpenGLPainter::directionalLightPop()
{
    pop(m_DirectionalLight, ShaderProgram::DirectionalLightUniform);
}

const QVector3D& OpenGLPainter::directionalLightTop() const
{
    return m_DirectionalLight.top();
}

int OpenGLPainter::directionalLightCount() const
{
    return m_DirectionalLight.count();
}

void OpenGLPainter::globalColorSetTop(const QColor &col)
{
    setTop(m_GlobalColor, ShaderProgram::ColorUniform, col);
}

void OpenGLPainter::globalColorPush()
{
    m_GlobalColor.push(m_GlobalColor.top());
}

void OpenGLPainter::globalColorPop()
{
    pop(m_GlobalColor, ShaderProgram::ColorUniform);
}

const QColor& OpenGLPainter::globalColorTop() const
{
    return m_GlobalColor.top();
}

int OpenGLPainter::globalColorCount() const
{
    return m_GlobalColor.count();
}

void OpenGLPainter::setToIdentity(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att)
{
    stack.top().setToIdentity();
    if ( m_bAutoUpdate )
        m_Shaders.top()->setUniformMatrix4(att, stack.top());
}

void OpenGLPainter::modelToWorldSetToIdentity()
{
    if ( m_ModelToWorld.top().isIdentity() ) return;
    setToIdentity(m_ModelToWorld, ShaderProgram::ModelToWorldMatrix);
}

void OpenGLPainter::worldToCameraSetToIdentity()
{
    if ( m_WorldToCamera.top().isIdentity() ) return;
    setToIdentity(m_WorldToCamera, ShaderProgram::WorldToCameraMatrix);
}

void OpenGLPainter::coordinateTransformSetToIdentity()
{
    if ( m_CoordinateTransform.top().isIdentity() ) return;
    setToIdentity(m_CoordinateTransform, ShaderProgram::CoordinateTransformMatrix);
}

void OpenGLPainter::cameraProjectionSetToIdentity()
{
    if ( m_CameraProjection.top().isIdentity() ) return;
    setToIdentity(m_CameraProjection, ShaderProgram::CameraProjectionMatrix);
}
