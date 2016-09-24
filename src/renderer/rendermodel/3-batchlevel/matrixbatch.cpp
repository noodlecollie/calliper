#include "matrixbatch.h"
#include <QtDebug>

namespace
{
    void copyItemDataIntoBuffer(const QVector<float>& data, char* buffer, int size, int& offset)
    {
        int dataBytes = data.count() * sizeof(float);
        Q_ASSERT_X(offset + dataBytes <= size, Q_FUNC_INFO, "GL buffer overflow when copying data!");

        memcpy(buffer + offset, data.constData(), dataBytes);
        offset += dataBytes;
    }

    void copyItemDataIntoBuffer(const QVector<quint32>& data, char* buffer, int size, quint32& indexDelta, int& offset)
    {
        int dataBytes = data.count() * sizeof(quint32);
        Q_ASSERT_X(offset + dataBytes <= size, Q_FUNC_INFO, "GL buffer overflow when copying data!");

        char* originalBase = buffer + offset;
        memcpy(originalBase, data.constData(), dataBytes);
        offset += dataBytes;

        quint32* indexBase = reinterpret_cast<quint32*>(originalBase);
        for ( int i = 0; i < data.count(); i++ )
        {
            indexBase[i] += indexDelta;
        }

        indexDelta += data.count();
    }

    void copyItemDataIntoBuffer(const NS_RENDERER::MatrixBatch::MatrixBatchItemPointer &item, char* buffer, int size,
                                int& positionOffset, int& normalOffset,
                                int& colorOffset, int& texCoordOffset)
    {
        copyItemDataIntoBuffer(item->m_Positions, buffer, size, positionOffset);
        copyItemDataIntoBuffer(item->m_Normals, buffer, size, normalOffset);
        copyItemDataIntoBuffer(item->m_Colors, buffer, size, colorOffset);
        copyItemDataIntoBuffer(item->m_TextureCoordinates, buffer, size, texCoordOffset);
    }
}

namespace NS_RENDERER
{
    MatrixBatch::MatrixBatch(const QMatrix4x4 &matrix)
        : m_matModelToWorld(matrix)
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
        MatrixBatchItemPointer item = MatrixBatchItemPointer::create();
        m_Items.insert(key, item);
        return item;
    }

    MatrixBatch::MatrixBatchItemPointer MatrixBatch::getItem(const MatrixBatchItemKey &key) const
    {
        return m_Items.value(key, MatrixBatchItemPointer());
    }

    void MatrixBatch::removeItem(const MatrixBatchItemKey &key)
    {
        m_Items.remove(key);
    }

    bool MatrixBatch::containsItem(const MatrixBatchItemKey &key) const
    {
        return m_Items.contains(key);
    }

    MatrixBatchItemMetadata MatrixBatch::buildItemMetadata() const
    {
        MatrixBatchItemMetadata meta;

        foreach ( MatrixBatchItemPointer item, m_Items )
        {
            meta += item->buildMetadata();
        }

        return meta;
    }

    void MatrixBatch::copyVertexDataIntoBuffer(char* buffer, int size, int &positionOffset, int &normalOffset,
                                         int &colorOffset, int &texCoordOffset) const
    {
        foreach ( const MatrixBatchItemPointer &item, m_Items.values() )
        {
            copyItemDataIntoBuffer(item, buffer, size,
                                   positionOffset, normalOffset, colorOffset, texCoordOffset);
        }
    }

    void MatrixBatch::copyIndexDataIntoBuffer(char *buffer, int size, quint32 &indexDelta, int &offset)
    {
        foreach ( const MatrixBatchItemPointer &item, m_Items.values() )
        {
            copyItemDataIntoBuffer(item->m_Indices, buffer, size, indexDelta, offset);
        }
    }

    const QMatrix4x4& MatrixBatch::matrix() const
    {
        return m_matModelToWorld;
    }

    void MatrixBatch::printDebugInfo() const
    {
        qDebug() << "Items:" << m_Items.count();
    }

    int MatrixBatch::itemCount() const
    {
        return m_Items.count();
    }
}
