#include "errorshader.h"
#include "calliperutil/qobject/qobjectutil.h"

namespace Model
{
    ErrorShader::ErrorShader(quint16 id, QObject* parent)
        : OpenGLShaderProgram(id, CalliperUtil::QObjectUtil::nonNamespacedClassName<ErrorShader>(), parent)
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

    Renderer::VertexFormat ErrorShader::vertexFormat() const
    {
        return Renderer::VertexFormat(4, 0, 0, 0);
    }

    int ErrorShader::maxBatchedItems() const
    {
        return 8;
    }
}
