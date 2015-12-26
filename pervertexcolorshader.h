#ifndef PERVERTEXCOLORSHADER_H
#define PERVERTEXCOLORSHADER_H

#include "shaderprogram.h"
#include <QMatrix4x4>

// Attributes required:
// - Position
// - Colour
// - Model to world matrix
// - World to camera matrix
// - Co-ordinate system matrix
// - Camera projection matrix
class PerVertexColorShader : public ShaderProgram
{
public:
    PerVertexColorShader();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;

private:
};

#endif // PERVERTEXCOLORSHADER_H
