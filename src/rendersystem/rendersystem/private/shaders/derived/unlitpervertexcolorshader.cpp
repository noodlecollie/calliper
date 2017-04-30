#include "unlitpervertexcolorshader.h"

UnlitPerVertexColorShader::UnlitPerVertexColorShader(QObject* parent)
    : Renderer::OpenGLShaderProgram(UnlitPerVertexColorShader::staticMetaObject::className(), parent)
{
}

UnlitPerVertexColorShader::~UnlitPerVertexColorShader()
{
}

void UnlitPerVertexColorShader::construct()
{
    addShaderFileWithCommonHeaders(QOpenGLShader::Vertex, ":/model/shaders/unlitpervertexcolorshader.vert");
    addShaderFromSourceFile(QOpenGLShader::Fragment, ":/model/shaders/unlitpervertexcolorshader.frag");
    link();
}

bool UnlitPerVertexColorShader::hasLocalUniformBlockBinding() const
{
    return true;
}

VertexFormat UnlitPerVertexColorShader::vertexFormat() const
{
    return Renderer::VertexFormat(4, 0, 4, 0);
}

int UnlitPerVertexColorShader::maxBatchedItems() const
{
    return 8;
}
