#include "textureplane.h"
#include <QQuaternion>

TexturePlane::TexturePlane()
{
    m_vecScale = QVector2D(1,1);
    m_vecTranslation = QVector2D(0,0);
    m_flRotation = 0;
    m_vecNormal = QVector3D(0,0,1);
    m_bAxesStale = true;
}

QString TexturePlane::texturePath() const
{
    return m_szTexturePath;
}

void TexturePlane::setTexturePath(const QString &path)
{
    m_szTexturePath = path;
}

QVector3D TexturePlane::uAxis() const
{
    if ( m_bAxesStale )
        updateAxes();

    return m_vecUAxis;
}

QVector3D TexturePlane::vAxis() const
{
    if ( m_bAxesStale )
        updateAxes();

    return m_vecVAxis;
}

QVector2D TexturePlane::translation() const
{
    return m_vecTranslation;
}

void TexturePlane::setTranslation(const QVector2D &tr)
{
    if ( tr == m_vecTranslation )
        return;

    m_vecTranslation = tr;
    m_bAxesStale = true;
}

QVector2D TexturePlane::scale() const
{
    return m_vecScale;
}

void TexturePlane::setScale(const QVector2D &sc)
{
    if ( sc == m_vecScale || m_vecScale.x() == 0 || m_vecScale.y() == 0 )
        return;

    m_vecScale = sc;
    m_bAxesStale = true;
}

float TexturePlane::rotation() const
{
    return m_flRotation;
}

void TexturePlane::setRotation(float rot)
{
    if ( rot == m_flRotation )
        return;

    m_flRotation = rot;
    m_bAxesStale = true;
}

QVector3D TexturePlane::normal() const
{
    return m_vecNormal;
}

void TexturePlane::setNormal(const QVector3D &n)
{
    Q_ASSERT(!n.isNull());
    if ( n == m_vecNormal || n.isNull() )
        return;

    m_vecNormal == n;
    m_bAxesStale = true;
}

void TexturePlane::updateAxes() const
{
    // Order of operations:
    // - Translation applied before rotation
    // - Translation applied before scale
    // - Scale applied before rotation
    // Therefore:
    // 1. Translate
    // 2. Scale
    // 3. Rotate

    // Assuming the plane normal is facing us:
    // - Translating in +X shifts the texture left on the plane.
    // - Translating in +Y shifts the texture up on the plane.
    // - Incrementing the rotation rotates the texture anticlockwise on the plane

    // Find the closest cardinal axis to use as the normal of the plane.
    float x = qAbs(m_vecNormal.x());
    float y = qAbs(m_vecNormal.y());
    float z = qAbs(m_vecNormal.z());

    // By default we use Z as the normal.
    Math::AxisIdentifier normalAxis = Math::AxisZ;

    // Simple check: see which value is the largest.
    // If the normal is null (which it shouldn't be!), the axis will be Z.
    if ( x > y && x > z )
        normalAxis = Math::AxisX;
    else if ( y > x && y > z )
        normalAxis = Math::AxisY;
    else
        normalAxis = Math::AxisZ;

    // Get the U and V axes.
    QVector3D uAxis, vAxis;
    uvAxes(normalAxis, uAxis, vAxis);

    // If the actual value of the normal is negative, flip the U axis.
    if ( m_vecNormal[normalAxis] < 0 )
        uAxis = -uAxis;

    // We now have some basic 3D axes to use.
    // Dot these with the normal to see whether they're perpendicular.
    float uDot = QVector3D::dotProduct(uAxis, m_vecNormal);
    float vDot = QVector3D::dotProduct(vAxis, m_vecNormal);

    // If the dot product is non-zero, the axis needs to be changed.
    // The dot value d gives the amount of the normal vector's direction that the vector v accounts for, between 0 and 1.
    // Therefore, a translation of -dn will add essentially translate back in the opposite direction to the normal
    // to correct for this.
    // The resulting vector, v-dn, will have no component in the normal direction; it just needs to be
    // normalised back to a unit vector.

    uAxis = (uAxis - (uDot * m_vecNormal)).normalized();
    vAxis = (vAxis - (vDot * m_vecNormal)).normalized();

    // The axes are now perpendicular to the normal of the plane.
    // Rotate them around the normal.
    QQuaternion qRot(m_flRotation, m_vecNormal);
    m_vecUAxis = qRot.rotatedVector(uAxis);
    m_vecVAxis = qRot.rotatedVector(vAxis);

    m_bAxesStale = false;
}

void TexturePlane::uvAxes(Math::AxisIdentifier axis, QVector3D &uAxis, QVector3D &vAxis)
{
    // To match Hammer, the texture is non-mirrored when the normal is one of the following:
    // +X
    // -Y
    // +Z

    switch (axis)
    {
    case Math::AxisX:
        uAxis = QVector3D(0,1,0);
        vAxis = QVector3D(0,0,1);
        return;

    case Math::AxisY:
        uAxis = QVector3D(1,0,0);
        vAxis = QVector3D(0,0,1);
        return;

    case Math::AxisZ:
        uAxis = QVector3D(1,0,0);
        vAxis = QVector3D(0,1,0);
        return;

    default:
        Q_ASSERT(false);
    }
}

QVector2D TexturePlane::textureCoordinate(const QVector3D &point, const QSize &textureSize) const
{
    // Method:
    // 1. Divide U and V by the appropriate scale factors.
    // 2. Project the input point to a 2D point by dotting it with both U and V.
    // 3. Convert the translation (in pixels) into texture units by dividing it componentwise by the texture size.
    //    Note that the texture size Y should be invertex to account for the flipped Y axis between pixels and texture units.
    // 4. Subtract the translation from the projected point.

    QVector3D u = uAxis();
    QVector3D v = vAxis();

    u /= m_vecScale.x();
    v /= m_vecScale.y();

    QVector2D projectedPoint(QVector3D::dot(point, u), QVector3D::dot(point, v));

    QVector2D tuTranslation(m_vecTranslation.x() / (float)textureSize.width(), m_vecTranslation.y() / (float)-textureSize.y());
    projectedPoint -= tuTranslation;

    return projectedPoint;
}
