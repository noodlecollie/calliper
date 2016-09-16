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

    MatrixBatch::MatrixBatchItemPointer MatrixBatch::createItem(const MatrixBatchItemKey &key)
    {
        // If the object already exists, the old shared pointer will be replaced
        // and will delete the batch item it was holding.
        m_Items.insert(key, MatrixBatchItemPointer(new MatrixBatchItem()));
    }

    MatrixBatch::MatrixBatchItemPointer MatrixBatch::getItem(const MatrixBatchItemKey &key) const
    {
        return m_Items.value(key, MatrixBatchItemPointer(NULL));
    }

    void MatrixBatch::removeItem(const MatrixBatchItemKey &key)
    {
        return m_Items.remove(key);
    }
}
