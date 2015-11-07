#include "csceneobject.h"
#include "callipermath.h"

CSceneObject::CSceneObject(CSceneObject *parent) : QObject(parent)
{
    m_vecPosition = QVector3D(0,0,0);
    m_bMatrixDirty = true;
}

QVector3D CSceneObject::position() const
{
    return m_vecPosition;
}

void CSceneObject::setPosition(const QVector3D &pos)
{
    if ( pos == m_vecPosition ) return;

    m_vecPosition = pos;
}

void CSceneObject::regenerateMatrices() const
{
    m_matParentToLocal = Math::matrixTranslate(-m_vecPosition) * Math::matrixOrientation(m_angAngles);
    m_matLocalToParent = m_matParentToLocal.inverted();

    m_bMatrixDirty = false;
}

QMatrix4x4 CSceneObject::matrixParentToLocal() const
{
    if ( m_bMatrixDirty ) regenerateMatrices();

    return m_matParentToLocal;
}

QMatrix4x4 CSceneObject::matrixLocalToParent() const
{
    if ( m_bMatrixDirty ) regenerateMatrices();

    return m_matLocalToParent;
}

CEulerAngle CSceneObject::angles() const
{
    return m_angAngles;
}

void CSceneObject::setAngles(const CEulerAngle &angle)
{
    if ( m_angAngles == angle ) return;

    m_angAngles = angle;
}

CVertexBundle* CSceneObject::vertexData() const
{
    // By default we have no vertex data (we could just be a grouping node).
    return NULL;
}
