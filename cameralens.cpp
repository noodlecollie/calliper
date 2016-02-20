#include "cameralens.h"
#include <QtMath>

const char* CameraLens::LensTypeNames[2] =
{
    "Orthographic",
    "Perspective",
};

CameraLens::CameraLens(LensType type)
{
    m_iType = type;

    m_flPFOV = 90.0f;
    m_flPAspectRatio = 1;
    m_flLeftPlane = -1;
    m_flRightPlane = 1;
    m_flTopPlane = 1;
    m_flBottomPlane = -1;
    m_flNearPlane = 1.0f;
    m_flFarPlane = 1000.0f;

    m_bMatrixDirty = true;
}

QMatrix4x4 CameraLens::perspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
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

QMatrix4x4 CameraLens::orthographicMatrix(float top, float bottom, float left, float right, float nearPlane, float farPlane)
{
    return QMatrix4x4((2.0f)/(right-left), 0, 0, -(right+left)/(right-left),
                      0, (2.0f)/(top-bottom), 0, -(top+bottom)/(top-bottom),
                      0, 0, -(2.0f)/(farPlane-nearPlane), -(farPlane+nearPlane)/(farPlane-nearPlane),
                      0, 0, 0, 1);
}

CameraLens::LensType CameraLens::type() const
{
    return m_iType;
}

void CameraLens::setType(LensType type)
{
    if ( type == m_iType ) return;

    m_iType = type;
    m_bMatrixDirty = true;
}

float CameraLens::fieldOfView() const
{
    return m_flPFOV;
}

void CameraLens::setFieldOfView(float fov)
{
    if ( fov == m_flPFOV ) return;

    m_flPFOV = fov;
    m_bMatrixDirty = true;
}

float CameraLens::aspectRatio() const
{
    return m_flPAspectRatio;
}

void CameraLens::setAspectRatio(float ratio)
{
    if ( ratio == m_flPAspectRatio ) return;

    m_flPAspectRatio = ratio;
    m_bMatrixDirty = true;
}

float CameraLens::leftPlane() const
{
    return m_flLeftPlane;
}

void CameraLens::setLeftPlane(float plane)
{
    if ( plane == m_flLeftPlane ) return;

    m_flLeftPlane = plane;
    m_bMatrixDirty = true;
}

float CameraLens::rightPlane() const
{
    return m_flRightPlane;
}

void CameraLens::setRightPlane(float plane)
{
    if ( plane == m_flRightPlane ) return;

    m_flRightPlane = plane;
    m_bMatrixDirty = true;
}

float CameraLens::topPlane() const
{
    return m_flTopPlane;
}

void CameraLens::setTopPlane(float plane)
{
    if ( plane == m_flTopPlane ) return;

    m_flTopPlane = plane;
    m_bMatrixDirty = true;
}


float CameraLens::bottomPlane() const
{
    return m_flBottomPlane;
}

void CameraLens::setBottomPlane(float plane)
{
    if ( plane == m_flBottomPlane ) return;

    m_flBottomPlane = plane;
    m_bMatrixDirty = true;
}

float CameraLens::nearPlane() const
{
    return m_flNearPlane;
}

void CameraLens::setNearPlane(float plane)
{
    if ( plane == m_flNearPlane ) return;

    m_flNearPlane = plane;
    m_bMatrixDirty = true;
}

float CameraLens::farPlane() const
{
    return m_flFarPlane;
}

void CameraLens::setFarPlane(float plane)
{
    if ( plane == m_flFarPlane ) return;

    m_flFarPlane = plane;
    m_bMatrixDirty = true;
}

void CameraLens::setPlanes(float left, float right, float top, float bottom, float near, float far)
{
    m_flLeftPlane = left;
    m_flRightPlane = right;
    m_flTopPlane = top;
    m_flBottomPlane = bottom;
    m_flNearPlane = near;
    m_flFarPlane = far;
    m_bMatrixDirty = true;
}

void CameraLens::setPlanes(const PlaneSet &planes)
{
    setPlanes(planes.leftPlane, planes.rightPlane, planes.topPlane, planes.bottomPlane, planes.nearPlane, planes.farPlane);
}

QMatrix4x4 CameraLens::projectionMatrix() const
{
    if ( m_bMatrixDirty ) regenerateMatrix();

    return m_matProjectionMatrix;
}

void CameraLens::regenerateMatrix() const
{
    switch ( m_iType )
    {
    case Orthographic:
        m_matProjectionMatrix = orthographicMatrix(m_flTopPlane, m_flBottomPlane, m_flLeftPlane, m_flRightPlane, m_flNearPlane, m_flFarPlane);
        break;

    case Perspective:
        m_matProjectionMatrix = perspectiveMatrix(m_flPFOV, m_flPAspectRatio, m_flNearPlane, m_flFarPlane);
        break;

    default:
        m_matProjectionMatrix.setToIdentity();
        break;
    }

    m_bMatrixDirty = false;
}

BoundingBox CameraLens::localViewVolumeBounds() const
{
    switch (m_iType)
    {
        case Orthographic:
        {
            // Return bounds in camera space.
            return BoundingBox(QVector3D(m_flLeftPlane, m_flBottomPlane, -m_flFarPlane),
                               QVector3D(m_flRightPlane, m_flTopPlane, -m_flNearPlane));
        }

        case Perspective:
        {
            PlaneSet pl = planes();
            return BoundingBox(QVector3D(pl.rightPlane, pl.topPlane, -pl.nearPlane),
                               QVector3D(pl.leftPlane, pl.bottomPlane, -pl.farPlane));

//            float top = m_flFarPlane * qTan(qDegreesToRadians(m_flPFOV/2.0f));
//            float bottom = -top;
//            float right = top*m_flPAspectRatio;
//            float left = -right;
//            return BoundingBox(QVector3D(right, top, -m_flNearPlane),
//                               QVector3D(left, bottom, -m_flFarPlane));
        }

        default:
        {
            return BoundingBox();
        }
    }
}

CameraLens::PlaneSet CameraLens::planes() const
{
    if ( m_iType == Perspective )
    {
        float top = m_flFarPlane * qTan(qDegreesToRadians(m_flPFOV/2.0f));
        float bottom = -top;
        float right = top*m_flPAspectRatio;
        float left = -right;
        return PlaneSet(left, right, top, bottom, m_flNearPlane, m_flFarPlane);
    }
    else
    {
        return PlaneSet(m_flLeftPlane, m_flRightPlane, m_flTopPlane, m_flBottomPlane, m_flNearPlane, m_flFarPlane);
    }
}

QVector3D CameraLens::mapPoint(const QPoint &pos, const QSize &viewSize) const
{
    // Adapted from original Qt code:
    // https://github.com/qtproject/qt3d/blob/qt4/src/threed/viewing/qglcamera.cpp

    int x = pos.x(), y = pos.y();
    int width = viewSize.width(), height = viewSize.height();

    float xrel, yrel;
    if (width)
        xrel = (((float)(x * 2)) - (float)width) / (float)width;
    else
        xrel = 0.0f;
    if (height)
        yrel = -(((float)(y * 2)) - (float)height) / (float)height;
    else
        yrel = 0.0f;

    QMatrix4x4 unprojection = projectionMatrix().inverted();

    // Return in camera space.
    return unprojection.map(QVector3D(xrel, yrel, -1.0f));
}
