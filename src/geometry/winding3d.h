#ifndef WINDING3D_H
#define WINDING3D_H

#include <QLinkedList>
#include <QVector3D>
#include "plane3d.h"
#include "ray3d.h"
#include <QList>
#include <QPair>
#include "windingvertex.h"

class Winding3D
{
public:
    explicit Winding3D(const Plane3D &plane);

    bool isNull() const;
    bool isClosed() const;

    QList<QVector3D> vertexList() const;
    QList<int> vertexIndices() const;

    Winding3D& clip(const Plane3D &clipPlane);
    Winding3D& clip(const QList<Plane3D> &clipPlanes);

    // Quite expensive!
    bool equivalentTo(const Winding3D &other, bool fuzzy = false) const;

    int vertexCount() const;
    QLinkedList<WindingVertex>::iterator verticesBegin();
    QLinkedList<WindingVertex>::const_iterator verticesBegin() const;
    QLinkedList<WindingVertex>::iterator verticesEnd();
    QLinkedList<WindingVertex>::const_iterator verticesEnd() const;

private:
    typedef QLinkedList<WindingVertex> VertexList;
    typedef QPair<VertexList::iterator, WindingVertex> VertexInsert;

    void createBlankWinding();
    void generateXAndY();
    Ray3D::IntersectionType splitEdgeWithPlane(const QVector3D &v0, const QVector3D &v1, const Plane3D &plane, QVector3D &intersection);
    void calculateEdgeSplit(const Plane3D &plane, VertexList::iterator &itV0, VertexList::iterator &itV1);
    void markVertexToDiscard(VertexList::iterator &vertex);
    void markVertexToInsert(VertexList::iterator &before, const QVector3D &position, bool prevEdgeOrig, bool nextEdgeOrig);
    void touchVertex(VertexList::iterator &vertex);
    void insertWaitingVertices();
    void removeDiscardedVertices();

    Plane3D                     m_Plane;
    QLinkedList<WindingVertex>  m_Vertices;
    QList<VertexInsert>         m_VerticesToInsert;
    QVector3D                   m_vecBasisX;
    QVector3D                   m_vecBasisY;
};

#endif // WINDING3D_H
