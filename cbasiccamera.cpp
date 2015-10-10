#include "cbasiccamera.h"
#include <QtMath>

const QMatrix4x4 HAMMER_TO_OPENGL(1,0,0,0,
                                  0,0,1,0,
                                  0,-1,0,0,
                                  0,0,0,1);

const QMatrix4x4 OPENGL_TO_HAMMER(1,0,0,0,
                                  0,0,-1,0,
                                  0,1,0,0,
                                  0,0,0,1);

CBasicCamera::CBasicCamera(QObject *parent) : QObject(parent)
{
    m_vecPosition = QVector3D(0,0,0);
    m_vecViewTarget = QVector3D(0,1,0);
    m_vecUpVector = QVector3D(0,0,1);
    m_bMatrixDirty = true;
}

QVector3D CBasicCamera::position() const
{
    return m_vecPosition;
}

void CBasicCamera::setPosition(const QVector3D &pos)
{
    if ( pos == m_vecPosition ) return;

    m_vecPosition = pos;
    m_bMatrixDirty = true;
    emit positionChanged(m_vecPosition);
}

QVector3D CBasicCamera::viewTarget() const
{
    return m_vecViewTarget;
}

void CBasicCamera::setViewTarget(const QVector3D &pos)
{
    if ( pos == m_vecViewTarget ) return;

    m_vecViewTarget = pos;
    m_bMatrixDirty = true;
    emit viewTargetChanged(m_vecViewTarget);
}

QVector3D CBasicCamera::upVector() const
{
    return m_vecUpVector;
}

void CBasicCamera::setUpVector(const QVector3D &up)
{
    if ( up == m_vecUpVector || up.isNull() ) return;

    Q_ASSERT(qFuzzyCompare(up.lengthSquared(), 1.0f));
    m_vecUpVector = up;
    m_bMatrixDirty = true;
    emit upVectorChanged(m_vecUpVector);
}

QMatrix4x4 CBasicCamera::worldToCamera() const
{
    if ( m_bMatrixDirty ) rebuildMatrices();
    return m_matWorldToCamera;
}

QMatrix4x4 CBasicCamera::cameraToWorld() const
{
    if ( m_bMatrixDirty ) rebuildMatrices();
    return m_matCameraToWorld;
}

void CBasicCamera::rebuildMatrices() const
{
    // http://schabby.de/view-matrix/

    // U, V and W define the camera space basis vectors.
    QVector3D w = (m_vecPosition - m_vecViewTarget).normalized();
    QVector3D u = QVector3D::crossProduct(m_vecUpVector, w);
    QVector3D v = QVector3D::crossProduct(w, u);
    const QVector3D& c = m_vecPosition;     // Convenience

    // The matrix is:
    m_matWorldToCamera = QMatrix4x4(u.x(), u.y(), u.z(),    -(c.x() * u.x()) - (c.y() * u.y()) - (c.z() * u.z()),
                                    v.x(), v.y(), v.z(),    -(c.x() * v.x()) - (c.y() * v.y()) - (c.z() * v.z()),
                                    w.x(), w.y(), w.z(),    -(c.x() * w.x()) - (c.y() * w.y()) - (c.z() * w.z()),
                                    0,0,0,1);
    m_matCameraToWorld = m_matWorldToCamera.inverted();

    m_bMatrixDirty = false;
}

void CBasicCamera::translateWorld(const QVector3D &delta)
{
    if ( delta.isNull() ) return;

    setPosition(position() + delta);
    setViewTarget(viewTarget() + delta);
}

void CBasicCamera::translateLocal(const QVector3D &delta)
{
    if ( delta.isNull() ) return;

    QVector3D fwd = (m_vecPosition - m_vecViewTarget).normalized();
    QVector3D right = QVector3D::crossProduct(fwd, m_vecUpVector);
    const QVector3D &up = m_vecUpVector;

    QVector3D translation = (delta.x() * right) + (delta.y() * fwd) + (delta.z() * up);
    setPosition(position() + translation);
    setViewTarget(viewTarget() + translation);
}

QMatrix4x4 CBasicCamera::perspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    float top = nearPlane * qTan((M_PI/180.0f) * (fov/2.0f));
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -right;

    return QMatrix4x4((2.0f*nearPlane)/(right-left), 0, (right+left)/(right-left), 0,
                      0, (2.0f*nearPlane)/(top-bottom), (top+bottom)/(top-bottom), 0,
                      0, 0, -(farPlane+nearPlane)/(farPlane-nearPlane), -(2.0f*farPlane*nearPlane)/(farPlane-nearPlane),
                      0, 0, -1, 0);
}

QMatrix4x4 CBasicCamera::orthographicMatrix(float top, float bottom, float left, float right, float nearPlane, float farPlane)
{
    return QMatrix4x4((2.0f)/(right-left), 0, 0, -(right+left)/(right-left),
                      0, (2.0f)/(top-bottom), 0, -(top+bottom)/(top-bottom),
                      0, 0, -(2.0f)/(farPlane-nearPlane), -(farPlane+nearPlane)/(farPlane-nearPlane),
                      0, 0, 0, 1);
}

const QMatrix4x4& CBasicCamera::coordsHammerToOpenGL()
{
    return HAMMER_TO_OPENGL;
}

const QMatrix4x4& CBasicCamera::coordsOpenGLToHammer()
{
    return OPENGL_TO_HAMMER;
}
