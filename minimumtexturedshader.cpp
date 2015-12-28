#include "minimumtexturedshader.h"
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"

MinimumTexturedShader::MinimumTexturedShader() : ShaderProgram(MinimumTexturedShader::staticName())
{
}

QString MinimumTexturedShader::staticName()
{
    return QString("MinimumTexturedShader");
}

void MinimumTexturedShader::construct()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    bool success = create();
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::VertexShader, ":/shaders/minimum_textured.vert");
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::FragmentShader, ":/shaders/minimum_textured.frag");
    Q_ASSERT(success);

    link();

    m_iAttributeLocations[Position] = f->glGetAttribLocation(handle(), "position");
    m_iAttributeLocations[UV] = f->glGetAttribLocation(handle(), "uv");
}

void MinimumTexturedShader::apply() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glUseProgram(handle());

    f->glEnableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glEnableVertexAttribArray(m_iAttributeLocations[UV]);
}

void MinimumTexturedShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDisableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glDisableVertexAttribArray(m_iAttributeLocations[UV]);
}
