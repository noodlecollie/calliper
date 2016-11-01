#include "unlitshader.h"
#include "general/generalutil.h"

namespace NS_MODEL
{
    UnlitShader::UnlitShader(quint16 id, QObject* parent)
        : OpenGLShaderProgram(id, NS_CALLIPERUTIL::General::nonNamespacedClassName<UnlitShader>(), parent)
    {

    }

    UnlitShader::~UnlitShader()
    {

    }

    void UnlitShader::construct()
    {
        addShaderFileWithCommonHeaders(QOpenGLShader::Vertex, ":/model/shaders/unlitshader.vert");
        addShaderFromSourceFile(QOpenGLShader::Fragment, ":/model/shaders/unlitshader.frag");
        link();
    }

    bool UnlitShader::hasLocalUniformBlockBinding() const
    {
        return true;
    }

    NS_RENDERER::VertexFormat UnlitShader::vertexFormat() const
    {
        return NS_RENDERER::VertexFormat(4, 0, 4, 2);
    }

    int UnlitShader::maxBatchedItems() const
    {
        return 8;
    }
}
