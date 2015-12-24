#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QObject>
#include <QScopedPointer>
#include "geometrydata.h"
#include <QMatrix4x4>

class SceneObject : public QObject
{
    Q_OBJECT
public:
    explicit SceneObject(SceneObject *parent = 0);

    // The SceneObject owns its geometry.
    // Any old geometry that is replaced will be deleted.
    GeometryData* geometry() const;
    void setGeometry(GeometryData* data);

    // Empty => This object has no geometry
    // (=> it's just for grouping children)
    bool isEmpty() const;

    // Matrices
    QMatrix4x4 parentToLocal() const;
    QMatrix4x4 localToParent() const;

    // Local
    QVector3D position() const;
    void setPosition(const QVector3D &pos);

signals:

public slots:

private:
    void rebuildMatrices() const;

    QScopedPointer<GeometryData>    m_pGeometry;

    mutable bool    m_bMatricesStale;
    QVector3D       m_vecPosition;

    mutable QMatrix4x4  m_matParentToLocal;
    mutable QMatrix4x4  m_matLocalToParent;
};

#endif // SCENEOBJECT_H
