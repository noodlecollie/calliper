#ifndef MINIMUMTEXTUREDSHADER_H
#define MINIMUMTEXTUREDSHADER_H

#include "shaderprogram.h"

class MinimumTexturedShader : public ShaderProgram
{
public:
    MinimumTexturedShader();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;

private:
    GLuint  m_iPositionLocation;
    GLuint  m_iUVLocation;
};

#endif // MINIMUMTEXTUREDSHADER_H
