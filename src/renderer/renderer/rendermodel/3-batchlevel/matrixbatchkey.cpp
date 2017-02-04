#include "matrixbatchkey.h"

namespace Renderer
{
    uint qHash(const MatrixBatchKey &key, uint seed)
    {
        return qHash(key.modelToWorldMatrix(), seed);
    }

    MatrixBatchKey::MatrixBatchKey(const QMatrix4x4 &matrix)
        : m_matModelToWorldMatrix(matrix)
    {

    }

    const QMatrix4x4& MatrixBatchKey::modelToWorldMatrix() const
    {
        return m_matModelToWorldMatrix;
    }
}
