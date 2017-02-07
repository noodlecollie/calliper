#include "unlitpervertexcolorshader.h"
#include "calliperutil/general/generalutil.h"

namespace Model
{
    UnlitPerVertexColorShader::UnlitPerVertexColorShader(quint16 id, QObject* parent)
        : OpenGLShaderProgram(id, CalliperUtil::General::nonNamespacedClassName<UnlitPerVertexColorShader>(), parent)
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

    Renderer::VertexFormat UnlitPerVertexColorShader::vertexFormat() const
    {
        return Renderer::VertexFormat(4, 0, 4, 0);
    }

    int UnlitPerVertexColorShader::maxBatchedItems() const
    {
        return 8;
    }
}