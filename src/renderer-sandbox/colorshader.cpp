#include "colorshader.h"

static const char *vertexShaderSource =
        "#version 410 core\n"
        "layout (location=0) in vec4 vPosition;\n"
        "layout (location=2) in vec4 vColour;\n"
        "layout (std140) uniform GlobalUniformBlock\n"
        "{\n"
        "   mat4 worldToCameraMatrix;\n"
        "   mat4 projectionMatrix;\n"
        "};\n"
        "layout (std140) uniform LocalUniformBlock\n"
        "{\n"
        "   mat4 modelToWorldMatrices[8];\n"
        "};\n"
        "mat4 coordTransformMatrix = mat4(1,0,0,0, 0,0,1,0, 0,-1,0,0, 0,0,0,1);\n"
        "out vec4 fColour;\n"
        "void main()\n"
        "{\n"
        "   uint id = uint(vPosition.w);\n"
        "   gl_Position = projectionMatrix * worldToCameraMatrix * modelToWorldMatrices[id] * vec4(vPosition.xyz, 1);\n"
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
    addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    link();
}
