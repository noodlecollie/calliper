#ifndef MATRIXBATCH_H
#define MATRIXBATCH_H

#include "renderer_global.h"
#include <QHash>
#include "rendermodel/4-batchitemlevel/matrixbatchitem.h"
#include <QSharedPointer>

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

        void clear();

        // Adds an item for the given object ID.
        // If an item already exists, it is replaced.
        MatrixBatchItemPointer insert(quint32 objectId);

        // Gets the item for the given object ID.
        // If it does not exist, NULL is returned.
        MatrixBatchItemPointer get(quint32 objectId) const;

        // Removes the item for the given object ID.
        // If the item doesn't exist, this function does nothing.
        void remove(quint32 objectId);

    private:
        QHash<quint32, MatrixBatchItemPointer>    m_Items;
    };
}

#endif // MATRIXBATCH_H
