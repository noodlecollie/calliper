#include "debugscreenspaceshader.h"

namespace NS_RENDERER
{
    DebugScreenSpaceShader::DebugScreenSpaceShader(QObject* parent)
        : OpenGLShaderProgram(QString(staticMetaObject.className()).remove(NS_STR_RENDERER "::"), parent)
    {

    }

    DebugScreenSpaceShader::~DebugScreenSpaceShader()
    {

    }

    void DebugScreenSpaceShader::construct()
    {
        addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/debugscreenspaceshader.vert");
        addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/debugscreenspaceshader.frag");
        link();
    }
}
