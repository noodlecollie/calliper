#ifndef WINDINGVERTEX_H
#define WINDINGVERTEX_H

#include "model_global.h"
#include <QVector3D>

namespace NS_MODEL
{
    class WindingVertex
    {
    public:
        WindingVertex(const QVector3D &vec, bool prevEdgeOriginal, bool nextEdgeOriginal);

        QVector3D position() const;
        void setPosition(const QVector3D &vec);

        bool shouldDiscard() const;
        void setShouldDiscard(bool discard);

        bool isPreviousEdgeOriginal() const;
        void setPreviousEdgeOriginal(bool orig);

        bool isNextEdgeOriginal() const;
        void setNextEdgeOriginal(bool orig);

        int index() const;
        void setIndex(int idx);

        inline bool operator == (const WindingVertex &other) const
        {
            return m_vecPosition == other.m_vecPosition &&
                    m_bShouldDiscard == other.m_bShouldDiscard &&
                    m_bPrevEdgeOriginal == other.m_bPrevEdgeOriginal &&
                    m_bNextEdgeOriginal == other.m_bNextEdgeOriginal &&
                    m_iIndex == other.m_iIndex;
        }

        inline bool operator != (const WindingVertex &other) const
        {
            return !(*this == other);
        }

        bool fuzzyEquals(const WindingVertex &other) const;

    private:
        QVector3D   m_vecPosition;
        bool        m_bShouldDiscard;
        bool        m_bPrevEdgeOriginal;
        bool        m_bNextEdgeOriginal;
        int         m_iIndex;
    };
}

#endif // WINDINGVERTEX_H
