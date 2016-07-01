#include "winding3d.h"
#include "callipermath.h"

class Winding3DEdgeHelper
{
public:


};

Winding3D::Winding3D(const Plane3D &plane) :
    m_Plane(plane)
{
    createBlankWinding();
}

bool Winding3D::isNull() const
{
    return m_Plane.isNull();
}

// To be closed, we must have no original vertices left,
// but we must not be empty.
bool Winding3D::isClosed() const
{
    if ( m_Vertices.count() < 1 )
        return false;

    for ( QLinkedList<Vertex>::const_iterator it = m_Vertices.begin();
          it != m_Vertices.end(); ++it)
    {
        if ( it->prevEdgeOriginal || it->nextEdgeOriginal )
            return false;
    }

    return true;
}

void Winding3D::createBlankWinding()
{
    m_Vertices.clear();

    if ( isNull() )
    {
        return;
    }

    // Get X and Y.
    generateXAndY();

    // Make dem HUGE.
    m_vecBasisX *= Math::CoordinateSystem::diagonal();
    m_vecBasisY *= Math::CoordinateSystem::diagonal();

    // Get the plane origin.
    QVector3D org = m_Plane.origin();

    // Generate four points.
    m_Vertices.append(Vertex(org - m_vecBasisX + m_vecBasisY, true, true));
    m_Vertices.append(Vertex(org + m_vecBasisX + m_vecBasisY, true, true));
    m_Vertices.append(Vertex(org + m_vecBasisX - m_vecBasisY, true, true));
    m_Vertices.append(Vertex(org - m_vecBasisX - m_vecBasisY, true, true));
}

void Winding3D::generateXAndY()
{
    if ( m_Plane.isNull() )
    {
        m_vecBasisX = QVector3D();
        m_vecBasisY = QVector3D();
        return;
    }

    // Find the largest axis of the normal.
    QVector3D nrm = m_Plane.normal();
    float largest = nrm.x();
    Math::AxisIdentifier largestAxis = Math::AxisX;
    for ( int i = 1; i < 3; i++ )
    {
        if ( nrm[i] > largest )
        {
            largest = nrm[i];
            largestAxis = (Math::AxisIdentifier)i;
        }
    }

    // Generate Y axis.
    m_vecBasisY = QVector3D();
    switch (largestAxis)
    {
        case Math::AxisZ:
            m_vecBasisY.setX(1);
            break;

        default:
            m_vecBasisY.setZ(1);
            break;
    }

    // Convert Y to lie in the plane.
    m_vecBasisY = Math::vectorPerpendicularTo(m_vecBasisY, nrm).normalized();

    // Generate an X axis using the cross product.
    m_vecBasisX = QVector3D::crossProduct(nrm, m_vecBasisY);
}

bool Winding3D::equivalentTo(const Winding3D &other) const
{
    // Plane must be the same.
    if ( m_Plane != other.m_Plane )
        return false;

    // There must be the same number of vertices.
    if ( m_Vertices.count() != other.m_Vertices.count() )
        return false;

    // Each vertex must have a corresponding entry in the other winding.
    for ( VertexList::const_iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it )
    {
        for ( VertexList::const_iterator it2 = other.m_Vertices.begin(); it2 != other.m_Vertices.end(); ++it2 )
        {
            if ( *it == *it2 )
                continue;
        }

        // We went through all the other vertices and none of them matched this one.
        return false;
    }

    // Everything matches up.
    return true;
}

Winding3D& Winding3D::clip(const Plane3D &clipPlane)
{
    // Go through each edge and try to split it.
    VertexList::iterator itV0 = m_Vertices.begin();
    VertexList::iterator itV1 = m_Vertices.begin();
    ++itV1;

    while ( true )
    {
        calculateEdgeSplit(clipPlane, itV0, itV1);

        // Increment the iterators.
        itV0++;
        if ( itV0 == m_Vertices.end() )
            itV0 = m_Vertices.begin();

        itV1++;
        if ( itV1 == m_Vertices.end() )
            itV1 = m_Vertices.begin();

        // We've done a full cycle.
        if ( itV0 == m_Vertices.begin() )
            break;
    }

    // After having checked all the edges, insert new vertices before discarding clipped ones.
    insertWaitingVertices();
    removeDiscardedVertices();

    return *this;
}

void Winding3D::calculateEdgeSplit(const Plane3D &plane, VertexList::iterator &itV0, VertexList::iterator &itV1)
{
    const QVector3D &v0 = itV0->point;
    const QVector3D &v1 = itV1->point;
    QVector3D intersectionPoint;
    Ray3D::IntersectionType intersectionType =
            splitEdgeWithPlane(itV0->point, itV1->point, plane, intersectionPoint);

    Plane3D::PointLocation plV0 = plane.getPointLocation(v0);
    Plane3D::PointLocation plV1 = plane.getPointLocation(v1);

    // If both vertices were on the same side of the plane, there was no intersection between them
    // even if the plane intersected the actual line somewhere.
    if ( plV0 == plV1 )
        intersectionType = Ray3D::NoIntersection;

    // If there was no intersection, discard any points that are in front of the plane.
    if ( intersectionType == Ray3D::NoIntersection )
    {
        Q_ASSERT_X(plV0 == plV1, Q_FUNC_INFO, "Points should both be on the same side of the plane.");
        Q_ASSERT_X(plV0 != Plane3D::OnPlane, Q_FUNC_INFO, "Points should not be on the plane.");
        Q_ASSERT_X(plV1 != Plane3D::OnPlane, Q_FUNC_INFO, "Points should not be on the plane.");

        if ( plV0 == Plane3D::InFrontOfPlane )
        {
            markVertexToDiscard(itV0);
        }
        if ( plV1 == Plane3D::InFrontOfPlane )
        {
            markVertexToDiscard(itV1);
        }

        if ( plV0 != plV1 )
        {
            qWarning().nospace() << Q_FUNC_INFO << " - Edge " << v0 << " -> " << v1
                                 << " reported no intersection with " << plane
                                 << " but both vertices are not on same side of plane "
                                 << "(" << Plane3D::enumPointLocationStringName(plV0)
                                 << " vs " << Plane3D::enumPointLocationStringName(plV1) << ")";
        }
    }
    // If there was an intersection, this could be that one point was on the plane and the other was
    // in front or behind, or that one point was in front of the plane and the other was behind.
    // In the first case, touch the point on the plane and remove the other point if it's in front.
    // In the second case, remove the vertex that's in front and create a new vertex.
    else if ( intersectionType == Ray3D::SingleIntersection )
    {
        Q_ASSERT_X(plV0 != plV1, Q_FUNC_INFO, "Points should not both be in same location on plane.");

        // No vertex is on the plane, so the plane splits the edge nicely.
        if ( plV0 != Plane3D::OnPlane && plV1 != Plane3D::OnPlane )
        {
            // If v0 is in front of the plane, the edge before the new vertex is definitely
            // not an original edge, and the edge after may be (depending on whether v0 says it was).
            // If v1 is in front of the plane, the edge after the new vertex is definitely
            // not an original edge, and the edge before may be (depending on whether v0 says it was).
            EdgeOriginalityPair origPair(false, false);
            if ( plV0 == Plane3D::InFrontOfPlane )
            {
                origPair.first = false;
                origPair.second = itV0->nextEdgeOriginal;
            }
            else
            {
                origPair.first = itV0->nextEdgeOriginal;
                origPair.second = false;
            }

            markVertexToDiscard(plV0 == Plane3D::InFrontOfPlane ? itV0 : itV1);
            markVertexToInsert(itV1, intersectionPoint, origPair);    // Insert in list before second vertex.
        }
        else
        {
            VertexList::iterator &vertexOnPlane = plV0 == Plane3D::OnPlane ? itV0 : itV1;
            VertexList::iterator &vertexNotOnPlane = plV0 == Plane3D::OnPlane ? itV1 : itV0;
            Plane3D::PointLocation &locOfVertexNotOnPlane = plV0 == Plane3D::OnPlane ? plV1 : plV0;

            touchVertex(vertexOnPlane);
            if ( locOfVertexNotOnPlane == Plane3D::InFrontOfPlane )
            {
                markVertexToDiscard(vertexNotOnPlane);
            }
        }

        if ( plV0 == plV1 )
        {
            qWarning().nospace() << Q_FUNC_INFO << " - Edge " << v0 << " -> " << v1
                                 << " reported intersection with " << plane
                                 << " but both vertices are in same location relative to the plane "
                                 << "(" << Plane3D::enumPointLocationStringName(plV0)
                                 << " vs " << Plane3D::enumPointLocationStringName(plV1) << ")";
        }
    }
    // If there were multiple intersections (ie. the plane and line were parallel),
    // don't change any of the points but do update them to say they're no longer original.
    else
    {
        Q_ASSERT_X(plV0 == Plane3D::OnPlane && plV1 == Plane3D::OnPlane,
                   Q_FUNC_INFO, "Points should both be on the plane.");

        touchVertex(itV0);
        touchVertex(itV1);

        if ( plV0 != Plane3D::OnPlane || plV1 != Plane3D::OnPlane )
        {
            qWarning().nospace() << Q_FUNC_INFO << " - Edge " << v0 << " -> " << v1
                                 << " lies in " << plane
                                 << " but one or more vertices were not on the plane "
                                 << "(" << Plane3D::enumPointLocationStringName(plV0)
                                 << " vs " << Plane3D::enumPointLocationStringName(plV1) << ")";
        }
    }

}

void Winding3D::markVertexToDiscard(VertexList::iterator &vertex)
{
    vertex->shouldDiscard = true;
}

void Winding3D::markVertexToInsert(VertexList::iterator &before, const QVector3D &position, const EdgeOriginalityPair originality)
{
    m_VerticesToInsert.append(VertexInsert(before, Vertex(position, originality.first, originality.second)));
}

void Winding3D::touchVertex(VertexList::iterator &vertex)
{
    vertex->prevEdgeOriginal = false;
    vertex->nextEdgeOriginal = false;
}

Ray3D::IntersectionType Winding3D::splitEdgeWithPlane(const QVector3D &v0, const QVector3D &v1, const Plane3D &plane, QVector3D &intersection)
{
    // Create a ray going from one vertex to the other.
    Ray3D ray(v0, v1-v0);

    // Find out whether it's split by the plane.
    Ray3D::IntersectionType intersectionType = Ray3D::NoIntersection;
    QVector3D pointOfSplit = ray.parameterise(plane, &intersectionType);

    if ( intersectionType == Ray3D::NoIntersection ||
         intersectionType == Ray3D::MultipleIntersections )
    {
        return intersectionType;
    }

    intersection = pointOfSplit;
    return intersectionType;
}

QList<QVector3D> Winding3D::vertexList() const
{
    QList<QVector3D> list;
    for ( VertexList::const_iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it )
    {
        list.append(it->point);
    }
    return list;
}

void Winding3D::insertWaitingVertices()
{
    for ( int i = 0; i < m_VerticesToInsert.count(); i++ )
    {
        const VertexInsert &item = m_VerticesToInsert.at(i);
        m_Vertices.insert(item.first, item.second);
    }

    m_VerticesToInsert.clear();
}

void Winding3D::removeDiscardedVertices()
{
    for ( VertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); /* nope */ )
    {
        if ( it->shouldDiscard )
        {
            it = m_Vertices.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

Winding3D& Winding3D::clip(const QList<Plane3D> &clipPlanes)
{
    foreach ( const Plane3D &clipPlane, clipPlanes )
    {
        clip(clipPlane);
    }

    return *this;
}
