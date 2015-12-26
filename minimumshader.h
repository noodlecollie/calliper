#ifndef MINIMUMSHADER_H
#define MINIMUMSHADER_H

#include "shaderprogram.h"

// Required attributes:
// - Position
// - Colour uniform
class MinimumShader : public ShaderProgram
{
public:
    MinimumShader();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;

private:
};

#endif // MINIMUMSHADER_H
