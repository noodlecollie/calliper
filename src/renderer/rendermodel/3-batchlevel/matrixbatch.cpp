#include "matrixbatch.h"

namespace NS_RENDERER
{
    MatrixBatch::MatrixBatch()
    {

    }

    MatrixBatch::~MatrixBatch()
    {
        clear();
    }

    void MatrixBatch::clear()
    {
        m_Items.clear();
    }

    MatrixBatch::MatrixBatchItemPointer MatrixBatch::insert(quint32 objectId)
    {
        // If the object already exists, the old shared pointer will be replaced
        // and will delete the batch item it was holding.
        m_Items.insert(objectId, MatrixBatchItemPointer(new MatrixBatchItem()));
    }

    MatrixBatch::MatrixBatchItemPointer MatrixBatch::get(quint32 objectId) const
    {
        return m_Items.value(objectId, MatrixBatchItemPointer(NULL));
    }

    void MatrixBatch::remove(quint32 objectId)
    {
        return m_Items.remove(objectId);
    }
}
