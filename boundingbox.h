#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector3D>
#include <QList>
#include <QMatrix4x4>

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const QVector3D &vMin, const QVector3D &vMax);

    bool operator==(const BoundingBox &other) const;
    bool operator!=(const BoundingBox &other) const;

    QVector3D min() const;
    void setMin(const QVector3D &vec);

    QVector3D max() const;
    void setMax(const QVector3D &vec);

    QVector3D centroid() const;

    QList<QVector3D> corners() const;
    BoundingBox transformed(const QMatrix4x4 &transform) const;

    bool isNull() const;

    // X alternates between min and max most frequently,
    // then Y and then Z. The list begins at min and ends at max.
    static QList<QVector3D> corners(const QVector3D &min, const QVector3D &max);

private:
    void sortVectors();
    QVector3D   m_vecMin;
    QVector3D   m_vecMax;
};

#endif // BOUNDINGBOX_H