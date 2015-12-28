#ifndef UNLITTEXTURESHADER_H
#define UNLITTEXTURESHADER_H

#include "shaderprogram.h"
#include <QMatrix4x4>

// Attributes required:
// - Position
// - UV
// - Model to world matrix
// - World to camera matrix
// - Co-ordinate system matrix
// - Camera projection matrix
class UnlitTextureShader : public ShaderProgram
{
public:
    UnlitTextureShader();

    static QString staticName();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;

private:
};

#endif // UNLITTEXTURESHADER_H
