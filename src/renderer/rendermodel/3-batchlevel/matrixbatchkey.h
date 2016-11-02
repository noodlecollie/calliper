#ifndef MATRIXBATCHKEY_H
#define MATRIXBATCHKEY_H

#include "renderer_global.h"
#include <QMatrix4x4>
#include "general/hashfunctions.h"

namespace Renderer
{
    class RENDERERSHARED_EXPORT MatrixBatchKey
    {
    public:
        explicit MatrixBatchKey(const QMatrix4x4 &matrix);

        const QMatrix4x4& modelToWorldMatrix() const;

        inline bool operator <(const MatrixBatchKey &other) const
        {
            return qHash(m_matModelToWorldMatrix) < qHash(other.m_matModelToWorldMatrix);
        }

        inline bool operator ==(const MatrixBatchKey &other) const
        {
            return m_matModelToWorldMatrix == other.m_matModelToWorldMatrix;
        }

        inline bool operator !=(const MatrixBatchKey &other) const
        {
            return !(*this == other);
        }

    private:
        QMatrix4x4    m_matModelToWorldMatrix;
    };

    RENDERERSHARED_EXPORT uint qHash(const MatrixBatchKey &key, uint seed = 0);
}

#endif // MATRIXBATCHKEY_H
