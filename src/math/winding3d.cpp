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
        if ( it->original )
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
    QVector3D xAxis, yAxis;
    generateXAndY(xAxis, yAxis);

    // Make dem HUGE.
    xAxis *= Math::CoordinateSystem::diagonal();
    yAxis *= Math::CoordinateSystem::diagonal();

    // Get the plane origin.
    QVector3D org = m_Plane.origin();

    // Generate four points.
    m_Vertices.append(Vertex(org - xAxis + yAxis));
    m_Vertices.append(Vertex(org + xAxis + yAxis));
    m_Vertices.append(Vertex(org + xAxis - yAxis));
    m_Vertices.append(Vertex(org - xAxis - yAxis));
}

void Winding3D::generateXAndY(QVector3D &xAxis, QVector3D &yAxis) const
{
    if ( m_Plane.isNull() )
    {
        xAxis = QVector3D();
        yAxis = QVector3D();
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
    yAxis = QVector3D();
    switch (largestAxis)
    {
        case Math::AxisZ:
            yAxis.setX(1);
            break;

        default:
            yAxis.setZ(1);
            break;
    }

    // Convert Y to lie in the plane.
    yAxis = Math::vectorPerpendicularTo(yAxis, nrm).normalized();

    // Generate an X axis using the cross product.
    xAxis = QVector3D::crossProduct(nrm, yAxis);
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

    QVector3D planeOrigin = clipPlane.origin();

    while ( true )
    {
        const QVector3D &v0 = itV0->point;
        const QVector3D &v1 = itV1->point;
        QVector3D intersectionPoint;
        Ray3D::IntersectionType intersectionType =
                splitEdgeWithPlane(itV0->point, itV1->point, clipPlane, intersectionPoint);

        bool v0InFrontOfPlane = QVector3D::dotProduct(clipPlane.normal(), v0 - planeOrigin) > 0;
        bool v1InFrontOfPlane = QVector3D::dotProduct(clipPlane.normal(), v1 - planeOrigin) > 0;

        // If both vertices were on the same side of the plane, there was no intersection between them.
        if ( v0InFrontOfPlane == v1InFrontOfPlane )
            intersectionType = Ray3D::NoIntersection;

        // If there was no intersection, discard any points that are in front of the plane.
        if ( intersectionType == Ray3D::NoIntersection )
        {
            // If one point is in front of the plane then they both are.
            if ( v0InFrontOfPlane )
            {
                itV0->shouldDiscard = true;
                itV1->shouldDiscard = true;
            }
        }
        // If there was an intersection, replace the point in front of the plane with the
        // new intersection point.
        else if ( intersectionType == Ray3D::SingleIntersection )
        {
            Vertex &v = v0InFrontOfPlane ? *itV0 : *itV1;
            v.point = intersectionPoint;
            v.original = false;
        }
        // If there were multiple intersections (ie. the plane and line were parallel),
        // don't change any of the points but do update them to say they're no longer original.
        else
        {
            itV0->original = false;
            itV1->original = false;
        }

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

    removeDiscardedVertices();

    return *this;
}

void Winding3D::removeDiscardedVertices()
{
    for ( VertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); /*nope*/ )
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
