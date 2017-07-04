#ifndef BATCHEDMODELMATRIXUNIFORMBLOCK_H
#define BATCHEDMODELMATRIXUNIFORMBLOCK_H

#include "rendersystem/private/opengl/uniforms/std140uniformstruct.h"
#include "rendersystem/private/opengl/uniforms/std140uniformelement.h"

template<quint32 SIZE>
struct BatchedModelMatrixUniformBlock : public Std140UniformStruct
{
    Std140UniformArray<QMatrix4x4, SIZE> modelToWorldMatrices;

    BatchedModelMatrixUniformBlock()
        : Std140UniformStruct(),
          modelToWorldMatrices(*this)
    {
    }

    static constexpr quint32 batchSize()
    {
        return SIZE;
    }
};

#endif // BATCHEDMODELMATRIXUNIFORMBLOCK_H
