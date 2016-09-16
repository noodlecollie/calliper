#include "matrixbatch.h"

namespace NS_RENDERER
{
    MatrixBatch::MatrixBatch()
    {

    }

    MatrixBatch::~MatrixBatch()
    {
        clearItems();
    }

    void MatrixBatch::clearItems()
    {
        m_Items.clear();
    }

    MatrixBatch::MatrixBatchItemPointer MatrixBatch::createItem(const MatrixBatchItemKey &key)
    {
        // If the object already exists, the old shared pointer will be replaced
        // and will delete the batch item it was holding.
        MatrixBatchItemPointer item(new MatrixBatchItem());
        m_Items.insert(key, item);
        return item;
    }

    MatrixBatch::MatrixBatchItemPointer MatrixBatch::getItem(const MatrixBatchItemKey &key) const
    {
        return m_Items.value(key, MatrixBatchItemPointer(NULL));
    }

    void MatrixBatch::removeItem(const MatrixBatchItemKey &key)
    {
        m_Items.remove(key);
    }

    bool MatrixBatch::containsItem(const MatrixBatchItemKey &key) const
    {
        return m_Items.contains(key);
    }
}
