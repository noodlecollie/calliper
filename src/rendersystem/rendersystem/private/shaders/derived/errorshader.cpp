#include "errorshader.h"

ErrorShader::ErrorShader(QObject* parent)
    : OpenGLShaderProgram(ErrorShader::staticMetaObject.className(), parent)
{

}

ErrorShader::~ErrorShader()
{

}

PrivateShaderDefs::ShaderId ErrorShader::shaderId()
{
    return PrivateShaderDefs::SimpleLitShaderId;
}

void ErrorShader::construct()
{
    addShaderFileWithCommonHeaders(QOpenGLShader::Vertex, ":/shaders/errorshader.vert");
    addShaderFileWithCommonHeaders(QOpenGLShader::Fragment, ":/shaders/errorshader.frag");
    link();
}

bool ErrorShader::hasLocalUniformBlockBinding() const
{
    return true;
}

VertexFormat ErrorShader::vertexFormat() const
{
    return VertexFormat(4, 0, 0, 0);
}

int ErrorShader::maxBatchedItems() const
{
    return UniformBlock::batchSize();
}
