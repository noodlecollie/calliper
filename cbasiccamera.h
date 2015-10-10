#ifndef CBASICCAMERA_H
#define CBASICCAMERA_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>

class CBasicCamera : public QObject
{
    Q_OBJECT
public:
    explicit CBasicCamera(QObject *parent = 0);

    QVector3D position() const;
    void setPosition(const QVector3D &pos);

    QVector3D viewTarget() const;
    void setViewTarget(const QVector3D &pos);

    QVector3D upVector() const;
    void setUpVector(const QVector3D &up);

    QMatrix4x4 worldToCamera() const;
    QMatrix4x4 cameraToWorld() const;

    void translateWorld(const QVector3D &delta);
    void translateLocal(const QVector3D &delta);

    static QMatrix4x4 perspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);
    static QMatrix4x4 orthographicMatrix(float top, float bottom, float left, float right, float nearPlane, float farPlane);
    static const QMatrix4x4& coordsHammerToOpenGL();
    static const QMatrix4x4& coordsOpenGLToHammer();

signals:
    void positionChanged(const QVector3D&);
    void viewTargetChanged(const QVector3D&);
    void upVectorChanged(const QVector3D&);

public slots:

private:
    void rebuildMatrices() const;

    QVector3D   m_vecPosition;
    QVector3D   m_vecViewTarget;
    QVector3D   m_vecUpVector;
    mutable bool        m_bMatrixDirty;
    mutable QMatrix4x4  m_matWorldToCamera;
    mutable QMatrix4x4  m_matCameraToWorld;
};

#endif // CBASICCAMERA_H
