#include "unlitpervertexcolorshader.h"
#include "calliperutil/qobject/qobjectutil.h"

namespace Model
{
    UnlitPerVertexColorShader::UnlitPerVertexColorShader(QObject* parent)
        : Renderer::OpenGLShaderProgram(KnownShaderDefs::UnlitPerVertexColourShaderId,
                                        CalliperUtil::QObjectUtil::nonNamespacedClassName<UnlitPerVertexColorShader>(),
                                        parent)
    {
    }

    UnlitPerVertexColorShader::~UnlitPerVertexColorShader()
    {
    }

    KnownShaderDefs::KnownShaderId UnlitPerVertexColorShader::knownShaderId() const
    {
        return static_cast<KnownShaderDefs::KnownShaderId>(shaderStoreId());
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
