#ifndef CAMERALENS_H
#define CAMERALENS_H

#include <QMatrix4x4>
#include "boundingbox.h"

class CameraLens
{
public:
    enum LensType
    {
        Orthographic,
        Perspective
    };

    struct PlaneSet
    {
        PlaneSet() : leftPlane(0), rightPlane(0), topPlane(0), bottomPlane(0), nearPlane(0), farPlane(0)
        {
        }
        PlaneSet(float l, float r, float t, float b, float n, float f) :
            leftPlane(l), rightPlane(r), topPlane(t), bottomPlane(b), nearPlane(n), farPlane(f)
        {
        }
        PlaneSet(const float* vals) : leftPlane(vals[0]), rightPlane(vals[1]), topPlane(vals[2]),
            bottomPlane(vals[3]), nearPlane(vals[4]), farPlane(vals[5])
        {
        }
        
        float leftPlane;
        float rightPlane;
        float topPlane;
        float bottomPlane;
        float nearPlane;
        float farPlane;
    };

    CameraLens(LensType type);
    QMatrix4x4 projectionMatrix() const;

    LensType type() const;
    void setType(LensType type);

    float fieldOfView() const;
    void setFieldOfView(float fov);

    float aspectRatio() const;
    void setAspectRatio(float ratio);

    float leftPlane() const;
    void setLeftPlane(float plane);

    float rightPlane() const;
    void setRightPlane(float plane);

    float topPlane() const;
    void setTopPlane(float plane);

    float bottomPlane() const;
    void setBottomPlane(float plane);

    float nearPlane() const;
    void setNearPlane(float plane);

    float farPlane() const;
    void setFarPlane(float plane);

    void setPlanes(float left, float right, float top, float bottom, float near, float far);
    void setPlanes(const PlaneSet &planes);
    PlaneSet planes() const;

    static QMatrix4x4 perspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);
    static QMatrix4x4 orthographicMatrix(float top, float bottom, float left, float right, float nearPlane, float farPlane);

    BoundingBox localViewVolumeBounds() const;

private:
    LensType    m_iType;

    float       m_flPFOV;
    float       m_flPAspectRatio;

    float       m_flTopPlane;
    float       m_flBottomPlane;
    float       m_flLeftPlane;
    float       m_flRightPlane;
    float       m_flNearPlane;
    float       m_flFarPlane;

    mutable bool        m_bMatrixDirty;
    mutable QMatrix4x4  m_matProjectionMatrix;

    void regenerateMatrix() const;
};

#endif // CAMERALENS_H
