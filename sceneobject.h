#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QObject>
#include <QScopedPointer>
#include "geometrydata.h"
#include <QMatrix4x4>
#include "eulerangle.h"

class SceneObject : public QObject
{
    Q_OBJECT
public:
    explicit SceneObject(SceneObject *parent = 0);
    SceneObject* parentObject() const;
    QList<SceneObject*> children() const;

    // The SceneObject owns its geometry.
    // Any old geometry that is replaced will be deleted.
    GeometryData* geometry() const;
    void setGeometry(GeometryData* data);

    // Empty => This object has no geometry
    // (=> it's just for grouping children)
    bool isEmpty() const;

    // Matrices
    // Model -> world = local -> parent = standard transforms
    // World -> camera = parent -> local = inverse transforms
    QMatrix4x4 parentToLocal() const;
    QMatrix4x4 localToParent() const;

    // Traverses the tree and calculates the root -> local matrix.
    QMatrix4x4 rootToLocal() const;

    // Local
    QVector3D position() const;
    void setPosition(const QVector3D &pos);

    // Translates in local co-ordinates - X is +forward/-backward,
    // Y is +left/-right, Z is +up/-down
    void translate(const QVector3D &trans);

    EulerAngle angles() const;
    void setAngles(const EulerAngle &angle);

signals:

public slots:

protected:
    virtual void rebuildLocalToParent() const;
    virtual void clampAngles();
    void rebuildMatrices() const;

    QScopedPointer<GeometryData>    m_pGeometry;

    mutable bool    m_bMatricesStale;
    QVector3D       m_vecPosition;
    EulerAngle      m_angAngles;

    mutable QMatrix4x4  m_matParentToLocal;
    mutable QMatrix4x4  m_matLocalToParent;
};

#endif // SCENEOBJECT_H
