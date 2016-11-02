#include "simplelitshader.h"

#include "general/generalutil.h"

namespace NS_MODEL
{
    SimpleLitShader::SimpleLitShader(quint16 id, QObject* parent)
        : OpenGLShaderProgram(id, NS_CALLIPERUTIL::General::nonNamespacedClassName<SimpleLitShader>(), parent)
    {

    }

    SimpleLitShader::~SimpleLitShader()
    {

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
}
