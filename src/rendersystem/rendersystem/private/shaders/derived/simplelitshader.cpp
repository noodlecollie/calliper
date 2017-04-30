#include "simplelitshader.h"

SimpleLitShader::SimpleLitShader(QObject* parent)
    : OpenGLShaderProgram(KnownShaderDefs::SimpleLitShaderId,
                                    SimpleLitShader::staticMetaObject.className(),
                                    parent)
{

}

SimpleLitShader::~SimpleLitShader()
{

}

KnownShaderDefs::KnownShaderId SimpleLitShader::knownShaderId() const
{
    return static_cast<KnownShaderDefs::KnownShaderId>(shaderStoreId());
}

void SimpleLitShader::construct()
{
    addShaderFileWithCommonHeaders(QOpenGLShader::Vertex, ":/model/shaders/simplelitshader.vert");
    addShaderFileWithCommonHeaders(QOpenGLShader::Fragment, ":/model/shaders/simplelitshader.frag");
    link();
}

bool SimpleLitShader::hasLocalUniformBlockBinding() const
{
    return true;
}

Renderer::VertexFormat SimpleLitShader::vertexFormat() const
{
    return Renderer::VertexFormat(4, 3, 4, 2);
}

int SimpleLitShader::maxBatchedItems() const
{
    return 8;
}
