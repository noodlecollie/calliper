#include "unlitshader.h"
#include "calliperutil/general/generalutil.h"

namespace Model
{
    UnlitShader::UnlitShader(quint16 id, QObject* parent)
        : OpenGLShaderProgram(id, CalliperUtil::General::nonNamespacedClassName<UnlitShader>(), parent)
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

    Renderer::VertexFormat UnlitShader::vertexFormat() const
    {
        return Renderer::VertexFormat(4, 0, 4, 2);
    }

    int UnlitShader::maxBatchedItems() const
    {
        return 8;
    }
}
