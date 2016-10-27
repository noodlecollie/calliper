#include "fuzzyvertexmap.h"
#include "math/math.h"
#include <QtMath>

namespace NS_MODEL
{
    namespace
    {
        inline bool floatsCloseEnough(float f1, float f2, float tolerance)
        {
            return qFabs(f1-f2) < tolerance;
        }

        inline bool vectorsCloseEnough(const QVector3D& vec1, const QVector3D& vec2, float tolerance)
        {
            return floatsCloseEnough(vec1.x(), vec2.x(), tolerance) &&
                    floatsCloseEnough(vec1.y(), vec2.y(), tolerance) &&
                    floatsCloseEnough(vec1.z(), vec2.z(), tolerance);
        }
    }

    FuzzyVertexMap::FuzzyVertexMap()
    {
        m_iNextIndex = 0;
        m_flTolerance = 0.01f;
    }

    int FuzzyVertexMap::count() const
    {
        return m_Map.count();
    }

    void FuzzyVertexMap::clear()
    {
        m_Map.clear();
        m_iNextIndex = 0;
    }

    int FuzzyVertexMap::mapToIndex(const QVector3D &vec)
    {
        for ( VertexMap::const_iterator it = m_Map.cbegin(); it != m_Map.cend(); ++it )
        {
            if ( vectorsCloseEnough(vec, it.key(), m_flTolerance) )
                return it.value();
        }

        m_Map.insert(vec, m_iNextIndex);
        m_iNextIndex++;
        return m_iNextIndex - 1;
    }

    QList<QVector3D> FuzzyVertexMap::vertexList() const
    {
        QList<QVector3D> list;
        list.reserve(m_Map.count());
        for ( int i = 0; i < m_Map.count(); i++ )
        {
            list.append(QVector3D());
        }

        for ( VertexMap::const_iterator it = m_Map.cbegin(); it != m_Map.cend(); ++it )
        {
            list[it.value()] = it.key();
        }

        return list;
    }

    float FuzzyVertexMap::tolerance() const
    {
        return m_flTolerance;
    }

    void FuzzyVertexMap::setTolerance(float t)
    {
        m_flTolerance = t;
    }
}
