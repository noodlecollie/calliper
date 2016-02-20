#ifndef HIERARCHICALOBJECT_H
#define HIERARCHICALOBJECT_H

#include <QObject>
#include <QVector3D>
#include "eulerangle.h"
#include <QMatrix4x4>
#include "iserialisable.h"

class HierarchicalObject : public QObject, public ISerialisable
{
    Q_OBJECT
    friend class UnserialisationFactory;
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

    // To convert lots of vectors, getting the matrix and doing it manually will probably be quicker,
    // especially with anything involving the root transform.
    QVector3D rootToLocal(const QVector3D &vec, bool direction = false) const;
    QVector3D localToRoot(const QVector3D &vec, bool direction = false) const;
    QVector3D parentToLocal(const QVector3D &vec, bool direction = false) const;
    QVector3D localToParent(const QVector3D &vec, bool direction = false) const;
    QVector3D parentToRoot(const QVector3D &vec, bool direction = false) const;
    QVector3D rootToParent(const QVector3D &vec, bool direction = false) const;

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

    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;

signals:

public slots:

protected:
    explicit HierarchicalObject(const QJsonObject &serialisedData, HierarchicalObject* parent = 0);

    virtual void rebuildLocalToParent() const;
    virtual void clampAngles();
    void rebuildMatrices() const;
    void initDefaults();

    mutable bool    m_bMatricesStale;
    QVector3D       m_vecPosition;
    EulerAngle      m_angAngles;
    QVector3D       m_vecScale;

    mutable QMatrix4x4  m_matParentToLocal;
    mutable QMatrix4x4  m_matLocalToParent;
};

#endif // HIERARCHICALOBJECT_H
