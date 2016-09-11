#include "scenecamera.h"
#include "callipermath.h"
#include <QtMath>
#include "geometryfactory.h"
#include "shaderstack.h"
#include "pervertexcolorshader.h"
#include "resourcemanager.h"
#include <QPoint>
#include <QSize>
#include <QJsonArray>
#include "basescene.h"

#define MAX_PITCH_DELTA 89.0f
#define MAX_ROLL_DELTA 180.0f

namespace NS_MODEL
{
    SceneCamera::SceneCamera(BaseScene *scene, SceneObject *parent) : SceneObject(scene, parent)
    {
        initDefaults();
    }

    void SceneCamera::initDefaults()
    {
        m_pLens.reset(new CameraLens(CameraLens::Perspective));
    }

    SceneCamera::SceneCamera(const SceneCamera &cloneFrom) : SceneObject(cloneFrom),
        m_pLens(new CameraLens(*cloneFrom.m_pLens.data()))
    {

    }

    SceneCamera::~SceneCamera()
    {
    }

    CameraLens* SceneCamera::lens() const
    {
        return m_pLens.data();
    }

    void SceneCamera::rebuildLocalToParent() const
    {
        // To get from local (model) space to world space,
        // we perform transforms forward.
        // To get from world space to camera space we must
        // perform the camera transforms backward.

        // Our default orientation should be looking down X,
        // but the OpenGL camera by default looks down (Hammer's) Y.
        // This means we need to apply an orientation first so that we face down
        // X before we do our other transforms.
        static const QMatrix4x4 defaultRot = Math::StaticMatrix::ROT_Z_270();

        SceneObject::rebuildLocalToParent();
        m_matLocalToParent = m_matLocalToParent * defaultRot;
    }

    void SceneCamera::clampAngles()
    {
        // These are slightly different to normal because the camera
        // needs to avoid the pitch singularities.

        if ( m_angAngles.pitch() < -MAX_PITCH_DELTA )
            m_angAngles.setPitch(-MAX_PITCH_DELTA);

        else if ( m_angAngles.pitch() > MAX_PITCH_DELTA )
            m_angAngles.setPitch(MAX_PITCH_DELTA);

        if ( m_angAngles.roll() < -MAX_ROLL_DELTA )
            m_angAngles.setRoll(-MAX_ROLL_DELTA);

        else if ( m_angAngles.roll() > MAX_ROLL_DELTA )
            m_angAngles.setRoll(MAX_ROLL_DELTA);

        m_angAngles.setYaw(std::fmod(m_angAngles.yaw(), 360.0f));
    }

    bool SceneCamera::scalable() const
    {
        // Scaling a camera doesn't make any sense, silly.
        return false;
    }

    QVector3D SceneCamera::worldTranslation(const QPoint &p0, const QPoint &p1, const QSize &viewSize, float distFromCamera) const
    {
    //    Get the camera-space directions corresponding to the two points.
    //    If the lens is perspective, these will diverge from the camera origin ((0,0,0) camera space).
    //    QVector3D cDir0 = lens()->mapPoint(p0, viewSize);
    //    QVector3D cDir1 = lens()->mapPoint(p1, viewSize);

    //    Get the camera->world matrix.
        QMatrix4x4 cameraToWorld = rootToLocal().inverted() * Math::StaticMatrix::OPENGL_TO_HAMMER();

    //    Translate the directions to world space.
    //    QVector3D wDir0 = (cameraToWorld * QVector4D(cDir0, 0)).toVector3D();
    //    QVector3D wDir1 = (cameraToWorld * QVector4D(cDir1, 0)).toVector3D();

    //    Calculate the new positions in world space.
    //    These lie along the lines that begin at the camera origin in the world
    //    and extend along each direction. If the lens is perspective, the further
    //    the distance from the camera the further separated the two points will be.
    //    QVector3D wPos0 = position() + (distFromCamera * wDir0);
    //    QVector3D wPos1 = position() + (distFromCamera * wDir1);

    //    Return the translation.
    //    return wPos1 - wPos0;

    //    In one line, this is:
         return (position() + (distFromCamera * ((cameraToWorld * QVector4D(lens()->mapPoint(p1, viewSize), 0)).toVector3D())))
                 - (position() + (distFromCamera * ((cameraToWorld * QVector4D(lens()->mapPoint(p0, viewSize), 0)).toVector3D())));
    }

    QVector3D SceneCamera::frustumDirection(const QPoint &p, const QSize &viewSize) const
    {
        return ((rootToLocal().inverted() * Math::StaticMatrix::OPENGL_TO_HAMMER()) * QVector4D(lens()->mapPoint(p, viewSize), 0)).toVector3D();
    }

    Ray3D SceneCamera::frustumRay(const QPoint &p, const QSize &viewSize) const
    {
        QVector3D rayDir = frustumDirection(p, viewSize);
        return Ray3D(position() + (lens()->nearPlane() * rayDir), rayDir);
    }

    bool SceneCamera::serialiseToJson(QJsonObject &obj) const
    {
        obj.insert(ISerialisable::KEY_IDENTIFIER(), QJsonValue(SceneCamera::serialiseIdentifier()));

        // Serialise the parent object first.
        QJsonObject jsonParent;
        SceneObject::serialiseToJson(jsonParent);

        // Insert this as the superclass.
        obj.insert(ISerialisable::KEY_SUPERCLASS(), QJsonValue(jsonParent));

        QJsonObject lensObj;
        m_pLens->serialiseToJson(lensObj);
        obj.insert("lens", QJsonValue(lensObj));

        return true;
    }

    SceneCamera::SceneCamera(BaseScene* scene, const QJsonObject &serialisedData, SceneObject *parent) :
        SceneObject(scene, serialisedData.value(ISerialisable::KEY_SUPERCLASS()).toObject(), parent)
    {
        initDefaults();

        if ( !validateIdentifier(serialisedData, SceneCamera::serialiseIdentifier()) )
            return;

        QJsonValue vLens = serialisedData.value("lens");
        if ( vLens.isObject() )
        {
            m_pLens->setFromJson(vLens.toObject());
        }
    }


    QString SceneCamera::serialiseIdentifier() const
    {
        return staticMetaObject.className();
    }

    QVector3D SceneCamera::forwardVector() const
    {
        return Math::angleToVectorSimple(angles());
    }
}