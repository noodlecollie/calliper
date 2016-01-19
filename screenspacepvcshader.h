#ifndef SCREENSPACEPVCSHADER_H
#define SCREENSPACEPVCSHADER_H

#include "shaderprogram.h"
#include <QMatrix4x4>

// Attributes required:
// - Position
// - Colour
// - Model to world matrix
// - World to camera matrix
// - Co-ordinate system matrix
// - Camera projection matrix
class ScreenSpacePVCShader : public ShaderProgram
{
public:
    ScreenSpacePVCShader();

    static QString staticName();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;
};

#endif // SCREENSPACEPVCSHADER_H
