#ifndef BASICLITTEXTURESHADER_H
#define BASICLITTEXTURESHADER_H

#include "shaderprogram.h"

// Attributes required:
// - Position
// - Normal
// - UV
// - Model to world matrix
// - World to camera matrix
// - Co-ordinate system matrix
// - Camera projection matrix
class BasicLitTextureShader : public ShaderProgram
{
public:
    BasicLitTextureShader();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;
};

#endif // BASICLITTEXTURESHADER_H
