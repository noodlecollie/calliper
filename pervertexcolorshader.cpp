#include "pervertexcolorshader.h"
#include <QOpenGLFunctions_4_1_Core>
#include "callipermath.h"
#include "resourcemanager.h"

PerVertexColorShader::PerVertexColorShader() : ShaderProgram("PerVertexColorShader")
{
    m_iPositionLocation = 0;
    m_iColourLocation = 0;
    m_iModelToWorldLocation = 0;
    m_iWorldToCameraLocation = 0;
    m_iHammerToOpenGLLocation = 0;
    m_iProjectionLocation = 0;
}

void PerVertexColorShader::construct()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    bool success = create();
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::VertexShader, ":/shaders/pervertexcolor.vert");
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::FragmentShader, ":/shaders/pervertexcolor.frag");
    Q_ASSERT(success);

    link();

    m_iPositionLocation = f->glGetAttribLocation(handle(), "vPositionModelSpace");
    m_iColourLocation = f->glGetAttribLocation(handle(), "vColor");
    m_iModelToWorldLocation = f->glGetUniformLocation(handle(), "modelToWorld");
    m_iWorldToCameraLocation = f->glGetUniformLocation(handle(), "worldToCamera");
    m_iHammerToOpenGLLocation = f->glGetUniformLocation(handle(), "hammerToOpenGL");
    m_iProjectionLocation = f->glGetUniformLocation(handle(), "projection");
}

void PerVertexColorShader::apply() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glUseProgram(handle());

    f->glEnableVertexAttribArray(m_iPositionLocation);
    f->glVertexAttribPointer(
       m_iPositionLocation,
       3,                   // size
       GL_FLOAT,            // type
       GL_FALSE,            // normalized?
       7*sizeof(float),     // stride
       (void*)0             // array buffer offset
    );

    f->glEnableVertexAttribArray(m_iColourLocation);
    f->glVertexAttribPointer(
       m_iColourLocation,
       4,                   // size
       GL_FLOAT,            // type
       GL_FALSE,            // normalized?
       7*sizeof(float),     // stride
       (void*)(3*sizeof(float))             // array buffer offset
    );

    f->glUniformMatrix4fv(m_iModelToWorldLocation, 1, GL_FALSE, m_matModelToWorld.constData());
    f->glUniformMatrix4fv(m_iWorldToCameraLocation, 1, GL_FALSE, m_matWorldToCamera.constData());
    f->glUniformMatrix4fv(m_iHammerToOpenGLLocation, 1, GL_FALSE, Math::hammerToOpenGL().constData());
    f->glUniformMatrix4fv(m_iProjectionLocation, 1, GL_FALSE, m_matCameraProjection.constData());
}

void PerVertexColorShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDisableVertexAttribArray(m_iPositionLocation);
    f->glDisableVertexAttribArray(m_iColourLocation);
}

void PerVertexColorShader::setModelToWorld(const QMatrix4x4 &mat)
{
    m_matModelToWorld = mat;
}

void PerVertexColorShader::setWorldToCamera(const QMatrix4x4 &mat)
{
    m_matWorldToCamera = mat;
}

void PerVertexColorShader::setCameraProjection(const QMatrix4x4 &mat)
{
    m_matCameraProjection = mat;
}
