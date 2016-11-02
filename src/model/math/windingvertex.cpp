#include "windingvertex.h"
#include "math/math.h"

namespace NS_MODEL
{
    WindingVertex::WindingVertex(const QVector3D &vec, bool prevEdgeOriginal, bool nextEdgeOriginal) :
        m_vecPosition(vec), m_bShouldDiscard(false), m_bPrevEdgeOriginal(prevEdgeOriginal),
        m_bNextEdgeOriginal(nextEdgeOriginal), m_iIndex(-1)
    {

    }

    QVector3D WindingVertex::position() const
    {
        return m_vecPosition;
    }

    void WindingVertex::setPosition(const QVector3D &vec)
    {
        m_vecPosition = vec;
    }

    bool WindingVertex::shouldDiscard() const
    {
        return m_bShouldDiscard;
    }

    void WindingVertex::setShouldDiscard(bool discard)
    {
        m_bShouldDiscard = discard;
    }

    bool WindingVertex::isPreviousEdgeOriginal() const
    {
        return m_bPrevEdgeOriginal;
    }

    void WindingVertex::setPreviousEdgeOriginal(bool orig)
    {
        m_bPrevEdgeOriginal = orig;
    }

    bool WindingVertex::isNextEdgeOriginal() const
    {
        return m_bNextEdgeOriginal;
    }

    void WindingVertex::setNextEdgeOriginal(bool orig)
    {
        m_bNextEdgeOriginal = orig;
    }

    int WindingVertex::index() const
    {
        return m_iIndex;
    }

    void WindingVertex::setIndex(int idx)
    {
        m_iIndex = idx;
    }

    bool WindingVertex::fuzzyEquals(const WindingVertex &other) const
    {
        return ((CalliperUtil::Math::fuzzyVectorIsNull(m_vecPosition) && CalliperUtil::Math::fuzzyVectorIsNull(other.m_vecPosition)) ||
                CalliperUtil::Math::fuzzyVectorEquals(m_vecPosition, other.m_vecPosition)) &&
                m_bShouldDiscard == other.m_bShouldDiscard &&
                m_bPrevEdgeOriginal == other.m_bPrevEdgeOriginal &&
                m_bNextEdgeOriginal == other.m_bNextEdgeOriginal &&
                m_iIndex == other.m_iIndex;
    }
}
