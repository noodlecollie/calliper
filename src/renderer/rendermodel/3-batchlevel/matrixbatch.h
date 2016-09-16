#ifndef MATRIXBATCH_H
#define MATRIXBATCH_H

#include "renderer_global.h"
#include <QHash>
#include "rendermodel/4-batchitemlevel/matrixbatchitem.h"
#include <QSharedPointer>
#include "rendermodel/4-batchitemlevel/matrixbatchitemkey.h"

namespace NS_RENDERER
{
    // This class groups all vertex data that is related to the same
    // model-to-world matrix.
    class MatrixBatch
    {
    public:
        typedef QSharedPointer<MatrixBatchItem> MatrixBatchItemPointer;

        MatrixBatch();
        ~MatrixBatch();

        void clearItems();

        // Creates an item for the given object ID.
        // If an item already exists, it is replaced.
        MatrixBatchItemPointer createItem(const MatrixBatchItemKey &key);

        // Gets the item for the given object ID.
        // If it does not exist, NULL is returned.
        MatrixBatchItemPointer getItem(const MatrixBatchItemKey &key) const;

        // Removes the item for the given object ID.
        // If the item doesn't exist, this function does nothing.
        void removeItem(const MatrixBatchItemKey &key);

        bool containsItem(const MatrixBatchItemKey &key) const;

    private:
        QHash<MatrixBatchItemKey, MatrixBatchItemPointer>    m_Items;
    };
}

#endif // MATRIXBATCH_H
