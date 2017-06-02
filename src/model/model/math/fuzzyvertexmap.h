#ifndef FUZZYVERTEXMAP_H
#define FUZZYVERTEXMAP_H

#include "model_global.h"
#include <QVector3D>
#include <QHash>
#include "calliperutil/hash/hashfunctions.h"

namespace Model
{
    class FuzzyVertexMap
    {
    public:
        FuzzyVertexMap();

        int mapToIndex(const QVector3D &vec);
        void clear();
        int count() const;

        float tolerance() const;
        void setTolerance(float t);

        QList<QVector3D> vertexList() const;

    private:
        typedef QHash<QVector3D, int> VertexMap;

        VertexMap   m_Map;
        int         m_iNextIndex;
        float       m_flTolerance;
    };
}

#endif // FUZZYVERTEXMAP_H
