#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector3D>
#include <QList>
#include <QMatrix4x4>
#include "callipermath.h"

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const QVector3D &vMin, const QVector3D &vMax);

    bool operator==(const BoundingBox &other) const;
    bool operator!=(const BoundingBox &other) const;

    QVector3D min() const;
    void setMin(const QVector3D &vec, bool sortAfter = true);

    QVector3D max() const;
    void setMax(const QVector3D &vec, bool sortAfter = true);

    QVector3D centroid() const;

    QList<QVector3D> corners() const;
    BoundingBox transformed(const QMatrix4x4 &mat) const; // Does not modify
    BoundingBox& transform(const QMatrix4x4 &mat);        // Does modify

    bool isNull() const;
    void setToNull();
    bool hasZeroVolume() const;

    BoundingBox unionCopy(const BoundingBox &other) const;  // Does not modify
    BoundingBox& unionWith(const BoundingBox &other);       // Does modify

    // X alternates between min and max most frequently,
    // then Y and then Z. The list begins at min and ends at max.
    static QList<QVector3D> corners(const QVector3D &min, const QVector3D &max);

    // Specifies which corner indexes between 0 and 7 are used for each face.
    // This is returned as a pointer to an array of 8 indexes.
    // The first 4 are the vertices for a face with a positive normal
    // in the given axis, and the second 4 are for a negative normal.
    // Vertices are specified in counter-clockwise winding.
    static const int* cornerVerticesForFace(Math::AxisIdentifier axis);

    BoundingBox centredOnOrigin() const;
    BoundingBox& centreOnOrigin();

    // Returns the width on the specific axis.
    float span(int axis) const;

    void sortVectors();

private:
    QVector3D   m_vecMin;
    QVector3D   m_vecMax;
    bool        m_bIsNull;
};

BoundingBox operator *(const QMatrix4x4 &mat, const BoundingBox &bbox);

#endif // BOUNDINGBOX_H
