#include "tempshader.h"
#include "shaders/shaderdefs.h"
#include "opengl/openglhelpers.h"
#include "opengl/openglerrors.h"

static const char *vertexShaderSource =
        "layout (location=0) in vec4 vPosition;\n"
        "layout (location=2) in vec4 vColour;\n"
        "layout (location=3) in vec2 vTexCoord;\n"
        "layout (std140) uniform LocalUniformBlock\n"
        "{\n"
        "   mat4 modelToWorldMatrices[8];\n"
        "};\n"
        "out vec4 fColour;\n"
        "out vec2 fTexCoord;\n"
        "void main()\n"
        "{\n"
        "   uint id = uint(vPosition.w);\n"
        "   gl_Position = projectionMatrix * COORD_TRANSFORM_HAMMER_OPENGL\n"
        "       *  worldToCameraMatrix * modelToWorldMatrices[id] * vec4(vPosition.xyz, 1);\n"
        "   fColour = vColour;\n"
        "   fTexCoord = vTexCoord;\n"
        "}\n"
    ;

static const char *fragmentShaderSource =
        "#version 410 core\n"
        "in vec4 fColour;\n"
        "in vec2 fTexCoord;\n"
        "layout(location = 0) out vec4 color;\n"
        "uniform sampler2D tex;\n"
        "void main()\n"
        "{\n"
        "   color = texture(tex, fTexCoord) * fColour;\n"
        "}\n"
    ;

using namespace Renderer;

TempShader::TempShader(quint16 id, QObject* parent)
    : OpenGLShaderProgram(id, QString(staticMetaObject.className()).remove(Renderer::STR_NAMESPACE).remove("::"), parent)
{

}

TempShader::~TempShader()
{

}

void TempShader::construct()
{
    addShaderWithCommonHeaders(QOpenGLShader::Vertex, vertexShaderSource);
    addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    link();
}
