#ifndef WINDING3D_H
#define WINDING3D_H

#include <QLinkedList>
#include <QVector3D>
#include "plane3d.h"
#include "ray3d.h"
#include <QList>
#include <QPair>

class Winding3D
{
public:
    explicit Winding3D(const Plane3D &plane);
    Winding3D(const Winding3D &other);

    bool isNull() const;
    bool isClosed() const;

    QList<QVector3D> vertexList() const;

    Winding3D& clip(const Plane3D &clipPlane);
    Winding3D& clip(const QList<Plane3D> &clipPlanes);

    // Quite expensive!
    bool equivalentTo(const Winding3D &other) const;

private:
    void createBlankWinding();
    void generateXAndY();
    Ray3D::IntersectionType splitEdgeWithPlane(const QVector3D &v0, const QVector3D &v1, const Plane3D &plane, QVector3D &intersection);

    class Vertex
    {
    public:
        explicit Vertex(const QVector3D &vec, bool prevEdgeOrig, bool nextEdgeOrig) :
            point(vec), shouldDiscard(false),
            prevEdgeOriginal(prevEdgeOrig), nextEdgeOriginal(nextEdgeOrig)
        {
        }

        inline bool operator == (const Vertex &other) const
        {
            return point == other.point && shouldDiscard == other.shouldDiscard &&
            prevEdgeOriginal == other.prevEdgeOriginal && nextEdgeOriginal == other.nextEdgeOriginal;
        }

        inline bool operator != (const Vertex &other) const
        {
            return !(*this == other);
        }

        QVector3D point;
        bool shouldDiscard;
        bool prevEdgeOriginal;
        bool nextEdgeOriginal;
    };

    friend class Winding3D::Vertex;
    typedef QLinkedList<Vertex> VertexList;
    typedef QPair<VertexList::iterator, Vertex> VertexInsert;
    typedef QPair<bool, bool> EdgeOriginalityPair;

    void calculateEdgeSplit(const Plane3D &plane, VertexList::iterator &itV0, VertexList::iterator &itV1);
    void markVertexToDiscard(VertexList::iterator &vertex);
    void markVertexToInsert(VertexList::iterator &before, const QVector3D &position, const EdgeOriginalityPair originality);
    void touchVertex(VertexList::iterator &vertex);
    void insertWaitingVertices();
    void removeDiscardedVertices();

    Plane3D                     m_Plane;
    QLinkedList<Vertex>         m_Vertices;
    QList<VertexInsert>         m_VerticesToInsert;
    QVector3D                   m_vecBasisX;
    QVector3D                   m_vecBasisY;
};

#endif // WINDING3D_H
