#ifndef MINIMUMTEXTUREDSHADER_H
#define MINIMUMTEXTUREDSHADER_H

#include "shaderprogram.h"

// Required attributes:
// - Position
// - UV
class MinimumTexturedShader : public ShaderProgram
{
public:
    MinimumTexturedShader();

    static QString staticName();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;

private:
};

#endif // MINIMUMTEXTUREDSHADER_H
