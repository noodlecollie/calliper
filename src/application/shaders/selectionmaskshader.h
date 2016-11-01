#ifndef SELECTIONMASKSHADER_H
#define SELECTIONMASKSHADER_H

#include "shaderprogram.h"

class SelectionMaskShader : public ShaderProgram
{
public:
    SelectionMaskShader();

    static QString staticName();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;
};

#endif // SELECTIONMASKSHADER_H
