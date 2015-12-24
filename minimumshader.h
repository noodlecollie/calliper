#ifndef MINIMUMSHADER_H
#define MINIMUMSHADER_H

#include "shaderprogram.h"

class MinimumShader : public ShaderProgram
{
public:
    MinimumShader();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;

private:
    GLuint  m_iColourLocation;
    GLuint  m_iPositionLocation;
};

#endif // MINIMUMSHADER_H
