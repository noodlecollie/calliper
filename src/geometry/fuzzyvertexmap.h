#ifndef FUZZYVERTEXMAP_H
#define FUZZYVERTEXMAP_H

#include <QVector3D>
#include <QHash>

class FuzzyVertexMap
{
public:
    FuzzyVertexMap();

    int mapToIndex(const QVector3D &vec);
    void clear();
    int count() const;

private:
    typedef QHash<QVector3D, int> VertexMap;

    VertexMap   m_Map;
    int         m_iNextIndex;
};

#endif // FUZZYVERTEXMAP_H
