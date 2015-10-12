#include "ccameralens.h"
#include <QtMath>

CCameraLens::CCameraLens(LensType type, QObject *parent) : QObject(parent)
{
    m_iType = type;

    m_flPFOV = 90.0f;
    m_flPAspectRatio = 1;
    m_flLeftPlane = -1;
    m_flRightPlane = 1;
    m_flTopPlane = 1;
    m_flBottomPlane = -1;
    m_flNearPlane = 0.1f;
    m_flFarPlane = 100.0f;

    m_bMatrixDirty = true;
}

QMatrix4x4 CCameraLens::perspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
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

QMatrix4x4 CCameraLens::orthographicMatrix(float top, float bottom, float left, float right, float nearPlane, float farPlane)
{
    return QMatrix4x4((2.0f)/(right-left), 0, 0, -(right+left)/(right-left),
                      0, (2.0f)/(top-bottom), 0, -(top+bottom)/(top-bottom),
                      0, 0, -(2.0f)/(farPlane-nearPlane), -(farPlane+nearPlane)/(farPlane-nearPlane),
                      0, 0, 0, 1);
}

CCameraLens::LensType CCameraLens::type() const
{
    return m_iType;
}

void CCameraLens::setType(LensType type)
{
    if ( type == m_iType ) return;

    m_iType = type;
    m_bMatrixDirty = true;
    emit lensChanged();
}

float CCameraLens::fieldOfView() const
{
    return m_flPFOV;
}

void CCameraLens::setFieldOfView(float fov)
{
    if ( fov == m_flPFOV ) return;

    m_flPFOV = fov;
    m_bMatrixDirty = true;
    emit lensChanged();
}

float CCameraLens::aspectRatio() const
{
    return m_flPAspectRatio;
}

void CCameraLens::setAspectRatio(float ratio)
{
    if ( ratio == m_flPAspectRatio ) return;

    m_flPAspectRatio = ratio;
    m_bMatrixDirty = true;
    emit lensChanged();
}

float CCameraLens::leftPlane() const
{
    return m_flLeftPlane;
}

void CCameraLens::setLeftPlane(float plane)
{
    if ( plane == m_flLeftPlane ) return;

    m_flLeftPlane = plane;
    m_bMatrixDirty = true;
    emit lensChanged();
}

float CCameraLens::rightPlane() const
{
    return m_flRightPlane;
}

void CCameraLens::setRightPlane(float plane)
{
    if ( plane == m_flRightPlane ) return;

    m_flRightPlane = plane;
    m_bMatrixDirty = true;
    emit lensChanged();
}

float CCameraLens::topPlane() const
{
    return m_flTopPlane;
}

void CCameraLens::setTopPlane(float plane)
{
    if ( plane == m_flTopPlane ) return;

    m_flTopPlane = plane;
    m_bMatrixDirty = true;
    emit lensChanged();
}


float CCameraLens::bottomPlane() const
{
    return m_flBottomPlane;
}

void CCameraLens::setBottomPlane(float plane)
{
    if ( plane == m_flBottomPlane ) return;

    m_flBottomPlane = plane;
    m_bMatrixDirty = true;
    emit lensChanged();
}

float CCameraLens::nearPlane() const
{
    return m_flNearPlane;
}

void CCameraLens::setNearPlane(float plane)
{
    if ( plane == m_flNearPlane ) return;

    m_flNearPlane = plane;
    m_bMatrixDirty = true;
    emit lensChanged();
}

float CCameraLens::farPlane() const
{
    return m_flFarPlane;
}

void CCameraLens::setFarPlane(float plane)
{
    if ( plane == m_flFarPlane ) return;

    m_flFarPlane = plane;
    m_bMatrixDirty = true;
    emit lensChanged();
}

void CCameraLens::setPlanes(float left, float right, float top, float bottom, float near, float far)
{
    m_flLeftPlane = left;
    m_flRightPlane = right;
    m_flTopPlane = top;
    m_flBottomPlane = bottom;
    m_flNearPlane = near;
    m_flFarPlane = far;
    m_bMatrixDirty = true;
    emit lensChanged();
}

QMatrix4x4 CCameraLens::projectionMatrix() const
{
    if ( m_bMatrixDirty ) regenerateMatrix();

    return m_matProjectionMatrix;
}

void CCameraLens::regenerateMatrix() const
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
