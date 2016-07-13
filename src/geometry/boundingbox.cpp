#include "boundingbox.h"
#include <QVector4D>

const int cornerVertexIndicesX[] = { 1, 3, 7, 5,   0, 4, 6, 2, };
const int cornerVertexIndicesY[] = { 3, 2, 6, 7,   0, 1, 5, 4, };
const int cornerVertexIndicesZ[] = { 4, 5, 7, 6,   0, 2, 3, 1, };

QDebug& operator <<(QDebug &debug, const BoundingBox &bbox)
{
    debug.nospace() << "BoundingBox(" << bbox.min() << ", " << bbox.max() << ")";
    return debug.space();
}

BoundingBox operator *(const QMatrix4x4 &mat, const BoundingBox &bbox)
{
    return bbox.transformed(mat);
}

BoundingBox::BoundingBox() :
    m_bIsNull(true)
{
}

BoundingBox::BoundingBox(const QVector3D &vMin, const QVector3D &vMax) :
    m_bIsNull(false)
{
    m_vecMin = vMin;
    m_vecMax = vMax;
    sortVectors();
}

void BoundingBox::sortVectors()
{
    if ( m_vecMin.x() > m_vecMax.x() )
    {
        float temp = m_vecMin.x();
        m_vecMin.setX(m_vecMax.x());
        m_vecMax.setX(temp);
    }

    if ( m_vecMin.y() > m_vecMax.y() )
    {
        float temp = m_vecMin.y();
        m_vecMin.setY(m_vecMax.y());
        m_vecMax.setY(temp);
    }

    if ( m_vecMin.z() > m_vecMax.z() )
    {
        float temp = m_vecMin.z();
        m_vecMin.setZ(m_vecMax.z());
        m_vecMax.setZ(temp);
    }
}

QVector3D BoundingBox::min() const
{
    return m_vecMin;
}

void BoundingBox::setMin(const QVector3D &vec, bool sortAfter)
{
    m_bIsNull = false;

    if ( m_vecMin == vec ) return;

    m_vecMin = vec;

    if ( sortAfter )
        sortVectors();
}

QVector3D BoundingBox::max() const
{
    return m_vecMax;
}

void BoundingBox::setMax(const QVector3D &vec, bool sortAfter)
{
    m_bIsNull = false;

    if ( m_vecMax == vec ) return;

    m_vecMax = vec;

    if ( sortAfter)
        sortVectors();
}

bool BoundingBox::isNull() const
{
    return m_bIsNull;
}

bool BoundingBox::hasZeroVolume() const
{
    return  isNull() ||
            qFuzzyIsNull(m_vecMax.x() - m_vecMin.x()) ||
            qFuzzyIsNull(m_vecMax.y() - m_vecMin.y()) ||
            qFuzzyIsNull(m_vecMax.z() - m_vecMin.z());
}

QList<QVector3D> BoundingBox::corners() const
{
    return isNull() ? QList<QVector3D>() : corners(m_vecMin, m_vecMax);
}

QList<QVector3D> BoundingBox::corners(const QVector3D &min, const QVector3D &max)
{
    QList<QVector3D> list;

    list.append(min);
    list.append(QVector3D(max.x(), min.y(), min.z()));
    list.append(QVector3D(min.x(), max.y(), min.z()));
    list.append(QVector3D(max.x(), max.y(), min.z()));
    list.append(QVector3D(min.x(), min.y(), max.z()));
    list.append(QVector3D(max.x(), min.y(), max.z()));
    list.append(QVector3D(min.x(), max.y(), max.z()));
    list.append(QVector3D(max));

    return list;
}

BoundingBox BoundingBox::transformed(const QMatrix4x4 &mat) const
{
    if ( isNull() )
        return BoundingBox();

    QList<QVector3D> list = corners();
    if ( list.count() < 1 )
    {
        QVector3D transformedVec = (mat * QVector4D()).toVector3D();
        return BoundingBox(transformedVec, transformedVec);
    }

    for ( int i = 0; i < list.count(); i++ )
    {
        list[i] = (mat * QVector4D(list[i],1)).toVector3D();
    }

    QVector3D min = list.at(0), max = list.at(0);
    for ( int i = 1; i < list.count(); i++ )
    {
        const QVector3D &v = list.at(i);

        if ( v.x() < min.x() )
            min.setX(v.x());
        if ( v.x() > max.x() )
            max.setX(v.x());

        if ( v.y() < min.y() )
            min.setY(v.y());
        if ( v.y() > max.y() )
            max.setY(v.y());

        if ( v.z() < min.z() )
            min.setZ(v.z());
        if ( v.z() > max.z() )
            max.setZ(v.z());
    }

    return BoundingBox(min,max);
}

BoundingBox& BoundingBox::transform(const QMatrix4x4 &mat)
{
    if ( !isNull() )
        *this = transformed(mat);

    return *this;
}

bool BoundingBox::operator ==(const BoundingBox &other) const
{
    return m_vecMin == other.m_vecMin &&
            m_vecMax == other.m_vecMax &&
            m_bIsNull == other.m_bIsNull;
}

bool BoundingBox::operator !=(const BoundingBox &other) const
{
    return !(*this == other);
}

QVector3D BoundingBox::centroid() const
{
    if ( isNull() )
        return QVector3D();

    return m_vecMin + ((m_vecMax-m_vecMin)/2.0f);
}

BoundingBox& BoundingBox::unionWith(const BoundingBox &other)
{
    if ( !isNull() )
        *this = unionCopy(other);

    return *this;
}

BoundingBox BoundingBox::unionCopy(const BoundingBox &other) const
{
    if ( isNull() )
        return other;

    if ( other.isNull() )
        return *this;

    return BoundingBox(QVector3D(qMin(m_vecMin.x(), other.m_vecMin.x()),
                                 qMin(m_vecMin.y(), other.m_vecMin.y()),
                                 qMin(m_vecMin.z(), other.m_vecMin.z())),
                       QVector3D(qMax(m_vecMax.x(), other.m_vecMax.x()),
                                 qMax(m_vecMax.y(), other.m_vecMax.y()),
                                 qMax(m_vecMax.z(), other.m_vecMax.z())));
}

const int* BoundingBox::cornerVerticesForFace(Math::AxisIdentifier axis)
{
    switch (axis)
    {
        case Math::AxisX:
            return cornerVertexIndicesX;

        case Math::AxisY:
            return cornerVertexIndicesY;

        case Math::AxisZ:
            return cornerVertexIndicesZ;

        default:
            Q_ASSERT(false);
            return NULL;
    }
}

float BoundingBox::span(int axis) const
{
    if ( isNull() )
        return 0.0f;

    return m_vecMax[axis] - m_vecMin[axis];
}

BoundingBox BoundingBox::centredOnOrigin() const
{
    if ( isNull() )
        return BoundingBox();

    QVector3D cent = centroid();
    QVector3D radius = (m_vecMax - m_vecMin)/2.0f;
    return BoundingBox(cent - radius, cent + radius);
}

BoundingBox& BoundingBox::centreOnOrigin()
{
    if ( !isNull() )
        *this = centredOnOrigin();

    return *this;
}

void BoundingBox::setToNull()
{
    m_bIsNull = true;
    m_vecMin = QVector3D();
    m_vecMax = QVector3D();
}
