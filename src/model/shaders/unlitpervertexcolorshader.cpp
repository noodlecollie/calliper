#include "unlitpervertexcolorshader.h"
#include "general/generalutil.h"

namespace NS_MODEL
{
    UnlitPerVertexColorShader::UnlitPerVertexColorShader(quint16 id, QObject* parent)
        : OpenGLShaderProgram(id, NS_CALLIPERUTIL::General::nonNamespacedClassName<UnlitPerVertexColorShader>(), parent)
    {

    }

    UnlitPerVertexColorShader::~UnlitPerVertexColorShader()
    {

    }

    void UnlitPerVertexColorShader::construct()
    {
        addVertexShaderFileWithCommonHeaders(":/model/shaders/unlitpervertexcolorshader.vert");
        addShaderFromSourceFile(QOpenGLShader::Fragment, ":/model/shaders/unlitpervertexcolorshader.frag");
        link();
    }

    bool UnlitPerVertexColorShader::hasLocalUniformBlockBinding() const
    {
        return true;
    }

    NS_RENDERER::VertexFormat UnlitPerVertexColorShader::vertexFormat() const
    {
        return NS_RENDERER::VertexFormat(4, 0, 4, 0);
    }

    int UnlitPerVertexColorShader::maxBatchedItems() const
    {
        return 8;
    }
}
