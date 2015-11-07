#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include "ceulerangle.h"

class CVertexBundle;

class CSceneObject : public QObject
{
    Q_OBJECT
public:
    explicit CSceneObject(CSceneObject *parent = 0);

    QVector3D position() const;
    void setPosition(const QVector3D &pos);

    CEulerAngle angles() const;
    void setAngles(const CEulerAngle &angle);

    QMatrix4x4 matrixParentToLocal() const;
    QMatrix4x4 matrixLocalToParent() const;

    virtual CVertexBundle* vertexData() const;

signals:

public slots:

private:
    void regenerateMatrices() const;

    QVector3D               m_vecPosition;
    CEulerAngle             m_angAngles;

    mutable bool            m_bMatrixDirty;
    mutable QMatrix4x4      m_matParentToLocal;
    mutable QMatrix4x4      m_matLocalToParent;
};

#endif // CSCENEOBJECT_H
