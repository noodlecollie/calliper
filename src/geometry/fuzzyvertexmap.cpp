#include "fuzzyvertexmap.h"
#include "callipermath.h"

FuzzyVertexMap::FuzzyVertexMap()
{
    m_iNextIndex = 0;
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
    bool isNull = Math::fuzzyVectorIsNull(vec);

    for ( VertexMap::const_iterator it = m_Map.cbegin(); it != m_Map.cend(); ++it )
    {
        if ( (isNull && Math::fuzzyVectorIsNull(it.key())) || Math::fuzzyVectorEquals(vec, it.key()) )
            return it.value();
    }

    m_Map.insert(vec, m_iNextIndex);
    m_iNextIndex++;
}
