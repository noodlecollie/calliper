#ifndef FUZZYVERTEXMAP_H
#define FUZZYVERTEXMAP_H

#include "model_global.h"
#include <QVector3D>
#include <QHash>
#include "general/hashfunctions.h"

namespace NS_MODEL
{
    class FuzzyVertexMap
    {
    public:
        FuzzyVertexMap();

        int mapToIndex(const QVector3D &vec);
        void clear();
        int count() const;

        QList<QVector3D> vertexList() const;

    private:
        typedef QHash<QVector3D, int> VertexMap;

        VertexMap   m_Map;
        int         m_iNextIndex;
    };
}

#endif // FUZZYVERTEXMAP_H
