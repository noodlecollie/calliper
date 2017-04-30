#include "simplelitshader.h"

SimpleLitShader::SimpleLitShader(QObject* parent)
    : OpenGLShaderProgram(SimpleLitShader::staticMetaObject.className(), parent)
{

}

SimpleLitShader::~SimpleLitShader()
{

}

void SimpleLitShader::construct()
{
    addShaderFileWithCommonHeaders(QOpenGLShader::Vertex, ":/shaders/simplelitshader.vert");
    addShaderFileWithCommonHeaders(QOpenGLShader::Fragment, ":/shaders/simplelitshader.frag");
    link();
}

bool SimpleLitShader::hasLocalUniformBlockBinding() const
{
    return true;
}

VertexFormat SimpleLitShader::vertexFormat() const
{
    return VertexFormat(4, 3, 4, 2);
}

int SimpleLitShader::maxBatchedItems() const
{
    return 8;
}
