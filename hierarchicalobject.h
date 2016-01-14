#ifndef HIERARCHICALOBJECT_H
#define HIERARCHICALOBJECT_H

#include <QObject>
#include <QVector3D>
#include "eulerangle.h"
#include <QMatrix4x4>

class HierarchicalObject : public QObject
{
    Q_OBJECT
public:
    explicit HierarchicalObject(HierarchicalObject *parent = 0);
    virtual ~HierarchicalObject();

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

    QVector3D scale() const;
    void setScale(const QVector3D &vec);
    virtual bool scalable() const;

    void lookAt(const QVector3D &pos);

signals:

public slots:

private:
    virtual void rebuildLocalToParent() const;
    virtual void clampAngles();
    void rebuildMatrices() const;

    mutable bool    m_bMatricesStale;
    QVector3D       m_vecPosition;
    EulerAngle      m_angAngles;
    QVector3D       m_vecScale;

    mutable QMatrix4x4  m_matParentToLocal;
    mutable QMatrix4x4  m_matLocalToParent;
};

#endif // HIERARCHICALOBJECT_H
