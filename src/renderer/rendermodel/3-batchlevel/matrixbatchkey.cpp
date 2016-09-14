#include "matrixbatchkey.h"

namespace NS_RENDERER
{
    MatrixBatchKey::MatrixBatchKey(const QMatrix4x4 &matrix)
        : m_matModelToWorldMatrix(matrix)
    {

    }

    const QMatrix4x4& MatrixBatchKey::modelToWorldMatrix() const
    {
        return m_matModelToWorldMatrix;
    }
}
