#ifndef BATCHEDMODELMATRIXUNIFORMBLOCK_H
#define BATCHEDMODELMATRIXUNIFORMBLOCK_H

#include "rendersystem/private/opengl/uniforms/std140uniformstruct.h"
#include "rendersystem/private/opengl/uniforms/std140uniformelement.h"

// TODO: Decide how to use this. Really we want a struct of arrays, not
// an array of structs. We should allow the shader to define arrays of
// a given length, and then create Std140UniformArrays that are of
// that length. This means that the length must be determined at runtime
// though, since we don't know what the shader is until then!
struct BatchedModelMatrixUniformBlock : public Std140UniformStruct
{
    Std140UniformElement<QMatrix4x4> modelToWorldMatrix;

    BatchedModelMatrixUniformBlock()
        : Std140UniformStruct(),
          modelToWorldMatrix(*this)
    {
    }
};

#endif // BATCHEDMODELMATRIXUNIFORMBLOCK_H
