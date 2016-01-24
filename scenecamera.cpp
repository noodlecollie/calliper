#include "scenecamera.h"
#include "callipermath.h"
#include <QtMath>
#include "geometryfactory.h"
#include "shaderstack.h"
#include "pervertexcolorshader.h"
#include "resourcemanager.h"
#include <QPoint>
#include <QSize>

#define MAX_PITCH_DELTA 89.0f
#define MAX_ROLL_DELTA 180.0f

SceneCamera::SceneCamera(SceneObject *parent) : SceneObject(parent),
    m_pLens(new CameraLens(CameraLens::Perspective)), m_pBoundsGeom(new GeometryData())
{
    Q_ASSERT(mapScene());
    m_LocalLensBounds = m_pLens->localViewVolumeBounds();
    rebuildViewBoundsGeometry();
}

SceneCamera::~SceneCamera()
{
}

bool SceneCamera::drawBounds() const
{
    return m_bDrawBounds;
}

void SceneCamera::setDrawBounds(bool enabled)
{
    m_bDrawBounds = enabled;
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
    static const QMatrix4x4 defaultRot = Math::matrixRotateZ(qDegreesToRadians(-90.0f));

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

void SceneCamera::rebuildViewBoundsGeometry()
{
    m_pBoundsGeom->clear();
    if ( m_LocalLensBounds.isNull() ) return;

    m_pBoundsGeom.reset(GeometryFactory::lineCuboid(m_LocalLensBounds, QColor::fromRgb(0xffff0000)));
    m_pBoundsGeom->setShaderOverride(PerVertexColorShader::staticName());
}

void SceneCamera::draw(ShaderStack *stack)
{
    BoundingBox bounds = lens()->localViewVolumeBounds();
    if ( bounds != m_LocalLensBounds )
    {
        m_LocalLensBounds = bounds;
        rebuildViewBoundsGeometry();
    }

    stack->modelToWorldPostMultiply(localToParent());

    if ( stack->camera() != this && m_bDrawBounds && !m_pBoundsGeom->isEmpty() )
    {
        stack->shaderPush(resourceManager()->shader(m_pBoundsGeom->shaderOverride()));
        stack->modelToWorldPush();
        stack->modelToWorldPostMultiply(Math::openGLToHammer());

        m_pBoundsGeom->upload();
        m_pBoundsGeom->bindVertices(true);
        m_pBoundsGeom->bindIndices(true);
        m_pBoundsGeom->applyDataFormat(stack->shaderTop());
        m_pBoundsGeom->draw();

        stack->modelToWorldPop();
        stack->shaderPop();
    }
}

QVector3D SceneCamera::mapPoint(const QPoint &pos, const QSize &viewSize) const
{
    // Firstly translate the point from window co-ordinates to device co-ordinates.
    QVector4D deviceCoords = Math::windowToDevice(viewSize.width(), viewSize.height()) * QVector4D(pos.x(), pos.y(), 0, 1);

    // Now un-project the point.
    // TODO: Will this work? We set (2,2) and (3,3) to 1 so that the matrix should be invertible.
    // Z and W don't really matter because we reset them anyway.
    QMatrix4x4 unprojection = lens()->projectionMatrix();
    unprojection(2,2) = 1;
    unprojection(3,3) = 1;

    bool success = false;
    unprojection = unprojection.inverted(&success);
    Q_ASSERT(success);

    QVector4D cameraCoords = unprojection * deviceCoords;
    cameraCoords.setZ(lens()->nearPlane());
    cameraCoords.setW(1);

    return (rootToLocal().inverted() * cameraCoords).toVector3D();
}

bool SceneCamera::scalable() const
{
    // Scaling a camera doesn't make any sense, silly.
    return false;
}
