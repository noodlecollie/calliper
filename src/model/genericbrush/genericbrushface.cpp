#include "genericbrushface.h"
#include "genericbrush.h"

namespace NS_MODEL
{
    GenericBrushFace::GenericBrushFace(GenericBrush* parentBrush)
        : m_pParentBrush(parentBrush)
    {
        Q_ASSERT_X(m_pParentBrush, Q_FUNC_INFO, "Parent brush cannot be null!");
    }

    GenericBrush* GenericBrushFace::parentBrush() const
    {
        return m_pParentBrush;
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
    }

    QVector<int> GenericBrushFace::indexList() const
    {
        return m_BrushVertexIndices;
    }

    void GenericBrushFace::clearIndices()
    {
        m_BrushVertexIndices.clear();
    }

    QVector<QVector3D> GenericBrushFace::referencedBrushVertexList() const
    {
        return m_pParentBrush->brushVertexList(m_BrushVertexIndices);
    }
}
