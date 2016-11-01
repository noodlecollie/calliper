#include "errorshader.h"
#include "general/generalutil.h"

namespace NS_MODEL
{
    ErrorShader::ErrorShader(quint16 id, QObject* parent)
        : OpenGLShaderProgram(id, NS_CALLIPERUTIL::General::nonNamespacedClassName<ErrorShader>(), parent)
    {

    }

    ErrorShader::~ErrorShader()
    {

    }

    void ErrorShader::construct()
    {
        addShaderFileWithCommonHeaders(QOpenGLShader::Vertex, ":/model/shaders/errorshader.vert");
        addShaderFromSourceFile(QOpenGLShader::Fragment, ":/model/shaders/errorshader.frag");
        link();
    }

    bool ErrorShader::hasLocalUniformBlockBinding() const
    {
        return true;
    }

    NS_RENDERER::VertexFormat ErrorShader::vertexFormat() const
    {
        return NS_RENDERER::VertexFormat(4, 0, 0, 0);
    }

    int ErrorShader::maxBatchedItems() const
    {
        return 8;
    }
}
