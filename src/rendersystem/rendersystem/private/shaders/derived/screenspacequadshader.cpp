#include "screenspacequadshader.h"
#include "rendersystem/private/shaders/common/vertexformat.h"

ScreenSpaceQuadShader::ScreenSpaceQuadShader(QObject *parent)
    : OpenGLShaderProgram(ScreenSpaceQuadShader::staticMetaObject.className(), parent)
{

}

ScreenSpaceQuadShader::~ScreenSpaceQuadShader()
{

}

PrivateShaderDefs::ShaderId ScreenSpaceQuadShader::shaderId()
{
    return PrivateShaderDefs::ScreenSpaceQuadShaderId;
}

void ScreenSpaceQuadShader::construct()
{
    addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/screenspacequadshader.vert");
    addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/screenspacequadshader.frag");
    link();
}

VertexFormat ScreenSpaceQuadShader::vertexFormat() const
{
    return VertexFormat(2, 0, 0, 0);
}

int ScreenSpaceQuadShader::maxBatchedItems() const
{
    return 1;
}
