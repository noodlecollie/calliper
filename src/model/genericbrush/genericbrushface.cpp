#include "genericbrushface.h"
#include "genericbrush.h"

namespace NS_MODEL
{
    GenericBrushFace::GenericBrushFace(GenericBrush* parentBrush)
        : QObject(parentBrush), m_pTexturePlane(new TexturePlane(this))
    {
        Q_ASSERT_X(parentBrush, Q_FUNC_INFO, "Parent brush cannot be null!");

        connect(m_pTexturePlane, &TexturePlane::dataChanged, this, &GenericBrushFace::dataChanged);
    }

    GenericBrush* GenericBrushFace::parentBrush() const
    {
        return qobject_cast<GenericBrush*>(parent());
    }

    int GenericBrushFace::indexAt(int index) const
    {
        if ( index < 0 || index >= m_BrushVertexIndices.count() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Index out of bounds!");
            return -1;
        }

        return m_BrushVertexIndices.at(index);
    }

    void GenericBrushFace::appendIndex(int i)
    {
        m_BrushVertexIndices.append(i);
        emit dataChanged();
    }

    int GenericBrushFace::indexCount() const
    {
        return m_BrushVertexIndices.count();
    }

    void GenericBrushFace::removeIndex(int index)
    {
        if ( index < 0 || index >= m_BrushVertexIndices.count() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Index out of bounds!");
            return;
        }

        m_BrushVertexIndices.remove(index);
        emit dataChanged();
    }

    QVector<int> GenericBrushFace::indexList() const
    {
        return m_BrushVertexIndices;
    }

    void GenericBrushFace::clearIndices()
    {
        m_BrushVertexIndices.clear();
        emit dataChanged();
    }

    QVector<QVector3D> GenericBrushFace::referencedBrushVertexList() const
    {
        return parentBrush()->brushVertexList(m_BrushVertexIndices);
    }
}
