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
    friend class Winding3DEdgeHelper;
public:
    explicit Winding3D(const Plane3D &plane);
    Winding3D(const Winding3D &other);

    bool isNull() const;
    bool isClosed() const;

    QList<QVector3D> vertexList() const;

    Winding3D& clip(const Plane3D &clipPlane);

    // Quite expensive!
    bool equivalentTo(const Winding3D &other) const;

private:
    void createBlankWinding();
    void generateXAndY(QVector3D &xAxis, QVector3D &yAxis) const;
    Ray3D::IntersectionType splitEdgeWithPlane(const QVector3D &v0, const QVector3D &v1, const Plane3D &plane, QVector3D &intersection);

    struct Vertex
    {
        explicit Vertex(const QVector3D &vec) :
            point(vec), original(true), shouldDiscard(false) {}

        inline bool operator == (const Vertex &other) const
        {
            return point == other.point && original == other.original;
        }

        inline bool operator != (const Vertex &other) const
        {
            return !(*this == other);
        }

        QVector3D point;
        bool original;
        bool shouldDiscard;
    };

    typedef QLinkedList<Vertex> VertexList;
    typedef QPair<VertexList::iterator, QVector3D> VertexInsert;

    void calculateEdgeSplit(const Plane3D &plane, VertexList::iterator &itV0, VertexList::iterator &itV1);
    void markVertexToDiscard(VertexList::iterator &vertex);
    void markVertexToInsert(VertexList::iterator &before, const QVector3D &position);
    void touchVertex(VertexList::iterator &vertex);
    void insertWaitingVertices();
    void removeDiscardedVertices();

    Plane3D                     m_Plane;
    QLinkedList<Vertex>         m_Vertices;
    QList<VertexInsert>         m_VerticesToInsert;
};

#endif // WINDING3D_H
