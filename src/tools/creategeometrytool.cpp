#include "creategeometrytool.h"
#include "mapdocument.h"
#include "mapscene.h"
#include "blockcreationhandle.h"
#include <QMouseEvent>
#include "application.h"
#include "viewport.h"
#include "mainwindow.h"
#include "callipermath.h"
#include "scenecamera.h"
#include "cameralens.h"
#include "mapgrid.h"
#include "crosshair3d.h"
#include "brushfactory.h"
#include "uiscene.h"
#include "uiblockcreationhandle.h"

CreateGeometryTool::CreateGeometryTool(MapDocument *document) : BaseTool(CreateGeometryTool::staticName(), document)
{
    m_pManipulator = NULL;
    m_pCrosshair = NULL;
    m_pBeginDragTarget = NULL;
    m_flBlockDepth = 64.0f;
    m_pUIBox = NULL;
}

CreateGeometryTool::~CreateGeometryTool()
{
    if ( m_bActive )
    {
        deactivate();
    }
}

QString CreateGeometryTool::staticName()
{
    return staticMetaObject.className();
}

void CreateGeometryTool::vActivate()
{
    BaseTool::vActivate();

    m_pManipulator = m_pDocument->scene()->createSceneObject<BlockCreationHandle>(m_pDocument->scene()->generalObjectsNode());
    m_pManipulator->setObjectName("_geometryCreationHandle");
    m_pManipulator->setHidden(true);

    m_pCrosshair = m_pDocument->scene()->createSceneObject<Crosshair3D>(m_pDocument->scene()->generalObjectsNode());
    m_pCrosshair->setObjectName("_geometryCreationCrosshair");
    m_pCrosshair->setHidden(true);

    m_pUIBox = m_pDocument->uiScene()->createSceneObject<UIBlockCreationHandle>(m_pDocument->uiScene()->generalObjectsNode());
    m_pUIBox->setObjectName("_geometryCreationWBox");
    m_pUIBox->setHidden(true);
}

void CreateGeometryTool::vDeactivate()
{
    m_pDocument->scene()->destroySceneObject(m_pManipulator);
    m_pManipulator = NULL;

    m_pDocument->scene()->destroySceneObject(m_pCrosshair);
    m_pCrosshair = NULL;

    m_pDocument->uiScene()->destroySceneObject(m_pUIBox);
    m_pUIBox = NULL;

    BaseTool::vDeactivate();
}

void CreateGeometryTool::vMousePress(QMouseEvent *e)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    SceneCamera* c = v->camera();
    if ( !v || !c )
        return;

    m_bInDrag = true;
    m_PosDragBegin = e->pos();
    m_flBlockDepth = m_pDocument->scene()->grid()->gridMultiple();

    m_pBeginDragTarget = v->pickObjectFromDepthBuffer(BaseScene::MapSceneFlag, m_PosDragBegin, SceneObject::EditableOnlyMask);
    Ray3D ray = c->frustumRay(m_PosDragBegin, v->size());

    if ( !m_pBeginDragTarget )
    {
        if ( !rayIntersectsZ0Plane(c, ray, m_vecDragBegin) )
        {
            m_bInDrag = false;
            return;
        }

        m_vecDragBeginClamped = m_vecDragBegin;
        Math::clampToNearestMultiple(m_vecDragBeginClamped, m_pDocument->scene()->grid()->gridMultiple(), Math::AxisFlagXYZ);

        m_PosDragCurrent = m_PosDragBegin;
        m_vecDragCurrent = m_vecDragBegin;

        updateBoundsDepth();
        updateManipulatorBounds();
    }
    else
    {
        // TODO
        m_bInDrag = false;
        return;
    }
}

void CreateGeometryTool::vMouseRelease(QMouseEvent *)
{
    endDrag();
}

void CreateGeometryTool::vMouseMove(QMouseEvent *e)
{
    if ( !m_bInDrag )
        return;

    Viewport* v = application()->mainWindow()->activeViewport();
    SceneCamera* c = v->camera();
    if ( !v || !c )
        return;

    m_PosDragCurrent = e->pos();

//    SceneObject* target = v->pickObjectFromDepthBuffer(BaseScene::MapSceneFlag, m_PosDragCurrent, SceneObject::EditableOnlyMask);
    Ray3D ray = c->frustumRay(m_PosDragCurrent, v->size());

    if ( /*!target*/ !m_pBeginDragTarget )
    {
        if ( !rayIntersectsZ0Plane(c, ray, m_vecDragCurrent) )
        {
            // Do nothing.
            return;
        }

        updateBoundsDepth();
        updateManipulatorBounds();
    }
    else
    {
        // TODO
    }

    updateCrosshairVisibility(e->pos());
}

void CreateGeometryTool::vMouseMoveHover(QMouseEvent *e)
{
    if ( m_bInDrag )
        return;

    if ( m_bMouseLookEnabled )
    {
        BaseTool::vMouseMoveHover(e);
    }

    updateCrosshairVisibility(e->pos());
}

void CreateGeometryTool::updateCrosshairVisibility(const QPoint &mousePos)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    SceneCamera* c = v->camera();
    if ( !v || !c )
    {
        m_pCrosshair->setHidden(true);
        return;
    }

    // TODO: Proper ray casting against objects;
    Ray3D ray = c->frustumRay(mousePos, v->size());
    QVector3D intersection;
    if ( !rayIntersectsZ0Plane(c, ray, intersection) )
    {
        m_pCrosshair->setHidden(true);
        return;
    }

    m_pCrosshair->setHidden(false);

    unsigned int gridMultiple = m_pDocument->scene()->grid()->gridMultiple();
    m_pCrosshair->setScale(QVector3D(gridMultiple, gridMultiple, gridMultiple));
    Math::clampToNearestMultiple(intersection, gridMultiple, Math::AxisFlagXYZ);
    m_pCrosshair->setPosition(intersection);
}

void CreateGeometryTool::endDrag()
{
    m_pBeginDragTarget = NULL;
    m_bInDrag = false;
    updateManipulatorBounds(true);

    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v )
        return;
    updateCrosshairVisibility(v->mapFromGlobal(QCursor::pos()));
}

bool CreateGeometryTool::rayIntersectsZ0Plane(SceneCamera *camera, const Ray3D &ray, QVector3D &intersection)
{
    Ray3D::IntersectionType intersectionType = Ray3D::NoIntersection;
    bool rayIntersectsZ0 = false;
    QVector3D potentialIntersection = ray.parameterise(Math::AxisZ, 0, &intersectionType);

    if ( intersectionType == Ray3D::SingleIntersection )
    {
        // Check if intersection was behind the origin of the ray.
        if ( QVector3D::dotProduct(ray.direction(), potentialIntersection - ray.origin()) <= 0.0f )
        {
            rayIntersectsZ0 = false;
        }

        // Check if the intersection was past the far plane of the camera.
        else if ( QVector3D::dotProduct(camera->forwardVector(), potentialIntersection - camera->position()) >= camera->lens()->farPlane() )
        {
            rayIntersectsZ0 = false;
        }
    }

    if ( !rayIntersectsZ0 )
    {
        return false;
    }

    intersection = potentialIntersection;
    return true;
}

void CreateGeometryTool::updateManipulatorBounds(bool endOfDrag)
{
    bool hidden = endOfDrag ? BoundingBox(m_vecDragBeginClamped, m_vecDragCurrentClamped).hasZeroVolume() : false;

    m_pManipulator->setHidden(hidden);
    m_pManipulator->setBounds(m_vecDragBeginClamped, m_vecDragCurrentClamped);

    m_pUIBox->setHidden(hidden);
    m_pUIBox->setBounds(m_vecDragBeginClamped, m_vecDragCurrentClamped);
}

void CreateGeometryTool::clearManipulator()
{
    m_pManipulator->setBounds(BoundingBox());
    m_pManipulator->setHidden(true);
    m_pUIBox->setBounds(BoundingBox());
    m_pUIBox->setHidden(true);

    m_vecDragBegin = QVector3D();
    m_vecDragBeginClamped = QVector3D();
    m_vecDragCurrent = QVector3D();
    m_vecDragCurrentClamped = QVector3D();
}

void CreateGeometryTool::vKeyPress(QKeyEvent *e)
{
    if ( m_bMouseLookEnabled )
    {
        BaseTool::vKeyPress(e);
        return;
    }

    if ( m_bInDrag )
        return;

    switch (e->key())
    {
        case Qt::Key_Escape:
        {
            clearManipulator();
            break;
        }

        case Qt::Key_Return:
        {
            createBlockBrush();
            break;
        }

        default:
            BaseTool::vKeyPress(e);
            break;
    }
}

void CreateGeometryTool::vWheel(QWheelEvent *e)
{
    if ( !m_bInDrag )
    {
        BaseTool::vWheel(e);
        return;
    }

    QPoint dAngle = e->angleDelta();
    float fraction = dAngle.y() / 120.0f;

    m_flBlockDepth += fraction * m_pDocument->scene()->grid()->gridMultiple();
    updateBoundsDepth();
    updateManipulatorBounds();
}

void CreateGeometryTool::updateBoundsDepth()
{
    m_vecDragCurrent[2] = m_vecDragBegin[2] + m_flBlockDepth;
    m_vecDragCurrentClamped = m_vecDragCurrent;
    Math::clampToNearestMultiple(m_vecDragCurrentClamped, m_pDocument->scene()->grid()->gridMultiple(), Math::AxisFlagXYZ);
}

void CreateGeometryTool::createBlockBrush()
{
    if ( m_bInDrag )
        return;

    BoundingBox bbox = m_pManipulator->bounds();
    if ( bbox.hasZeroVolume() )
        return;

    // TODO: Textures.
    BrushFactory::fromBoundingBox(m_pDocument->scene(), m_pDocument->scene()->generalObjectsNode(), bbox, "/textures/dev/dev_purple001", bbox.centroid());
    clearManipulator();
}
