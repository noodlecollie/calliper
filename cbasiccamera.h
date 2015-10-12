#ifndef CBASICCAMERA_H
#define CBASICCAMERA_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>

class CCameraLens;

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

    // Camera owns the lens.
    CCameraLens* lens() const;
    void setLens(CCameraLens* lens);

    QMatrix4x4 worldToCamera() const;
    QMatrix4x4 cameraToWorld() const;

    void translateWorld(const QVector3D &delta);
    void translateLocal(const QVector3D &delta);

    static const QMatrix4x4& coordsHammerToOpenGL();
    static const QMatrix4x4& coordsOpenGLToHammer();

signals:
    void positionChanged(const QVector3D&);
    void viewTargetChanged(const QVector3D&);
    void upVectorChanged(const QVector3D&);
    void lensChanged(CCameraLens*);

public slots:

private:
    void rebuildMatrices() const;

    QVector3D   m_vecPosition;
    QVector3D   m_vecViewTarget;
    QVector3D   m_vecUpVector;
    mutable bool        m_bMatrixDirty;
    mutable QMatrix4x4  m_matWorldToCamera;
    mutable QMatrix4x4  m_matCameraToWorld;
    CCameraLens*        m_pLens;
};

#endif // CBASICCAMERA_H
