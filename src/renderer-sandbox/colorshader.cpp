#include "colorshader.h"

static const char *vertexShaderSource =
        "layout (location=0) in vec4 vPosition;\n"
        "layout (location=2) in vec4 vColour;\n"
        "layout (std140) uniform LocalUniformBlock\n"
        "{\n"
        "   mat4 modelToWorldMatrices[8];\n"
        "};\n"
        "out vec4 fColour;\n"
        "void main()\n"
        "{\n"
        "   uint id = uint(vPosition.w);\n"
        "   gl_Position = projectionMatrix * COORD_TRANSFORM_HAMMER_OPENGL\n"
        "       * worldToCameraMatrix * modelToWorldMatrices[id] * vec4(vPosition.xyz, 1);\n"
        "   fColour = vColour;\n"
        "}\n"
    ;

static const char *fragmentShaderSource =
        "#version 410 core\n"
        "in vec4 fColour;\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = fColour;\n"
        "}\n"
    ;

using namespace NS_RENDERER;

ColorShader::ColorShader(quint16 id, QObject *parent)
    : OpenGLShaderProgram(id, QString(staticMetaObject.className()).remove(NS_STR_RENDERER "::"), parent)
{

}

ColorShader::~ColorShader()
{

}

void ColorShader::construct()
{
    addVertexShaderWithCommonHeaders(vertexShaderSource);
    addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    link();
}
