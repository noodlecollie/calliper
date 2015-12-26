#include "minimumshader.h"
#include <QtDebug>
#include "resourcemanager.h"
#include "openglrenderer.h"

MinimumShader::MinimumShader() : ShaderProgram("MinimumShader")
{
}

void MinimumShader::construct()
{
    bool success = create();
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::VertexShader, ":/shaders/minimum.vert");
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::FragmentShader, ":/shaders/minimum.frag");
    Q_ASSERT(success);

    link();
    m_iAttributeLocations[ColorUniform] = resourceManager()->functions()->glGetUniformLocation(handle(), "inCol");
    m_iAttributeLocations[Position] = resourceManager()->functions()->glGetAttribLocation(handle(), "position");
}

void MinimumShader::apply() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glUseProgram(handle());

    f->glEnableVertexAttribArray(m_iAttributeLocations[Position]);
}

void MinimumShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDisableVertexAttribArray(m_iAttributeLocations[Position]);
}
