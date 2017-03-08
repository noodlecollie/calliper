#include "simplelitshader.h"
#include "calliperutil/qobject/qobjectutil.h"
namespace Model
{
    SimpleLitShader::SimpleLitShader(QObject* parent)
        : Renderer::OpenGLShaderProgram(KnownShaderDefs::SimpleLitShaderId,
                                        CalliperUtil::QObjectUtil::nonNamespacedClassName<SimpleLitShader>(),
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
}
