#include "scaletool.h"
#include "scalehandle.h"
#include "mapdocument.h"
#include "uiscene.h"
#include "viewport.h"
#include "application.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include "mapscene.h"
#include "scenecamera.h"
#include "mapgrid.h"

ScaleTool::ScaleTool(MapDocument *document) : BaseTool(ScaleTool::staticName(), document)
{
    m_pHandle = NULL;
    m_bInMove = false;
}

ScaleTool::~ScaleTool()
{
    if ( m_bActive )
    {
        deactivate();
    }
}

QString ScaleTool::staticName()
{
    return QString("ScaleTool");
}

void ScaleTool::vActivate()
{
    m_pHandle = new ScaleHandle(m_pDocument->uiScene()->root());
    m_pHandle->setObjectName("_scaleHandle");
    m_vecScale = QVector3D(1,1,1);
    updateTableFromSet();
    updateHandleState();
    m_bInMove = false;
}

void ScaleTool::vDeactivate()
{
    if ( m_bInMove )
        endMove();

    clearTableManipulators();
    delete m_pHandle;
    m_pHandle = NULL;
    BaseTool::vDeactivate();
}

void ScaleTool::vKeyPress(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    BaseTool::vKeyPress(e);
}

void ScaleTool::vKeyRelease(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    BaseTool::vKeyRelease(e);
}

void ScaleTool::vSelectedSetChanged()
{
    if ( m_bInMove )
        endMove();

    updateTableFromSet();
    updateHandleState();
    BaseTool::vSelectedSetChanged();
}


void ScaleTool::vMousePress(QMouseEvent *e)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v || !v->camera() )
        return;

    QRgb col = 0xffffffff;
    SceneObject* obj = v->pickObjectFromDepthBuffer(MapDocument::UISceneFlag | MapDocument::MapSceneFlag, e->pos(), &col);

    if ( !obj || obj->scene() == m_pDocument->scene() )
    {
        addToSelectedSet(obj, !m_flKBModifiers.testFlag(Qt::ControlModifier));
        return;
    }

    if ( obj != m_pHandle )
    {
        m_pDocument->selectedSetClear();
        return;
    }

    // Cache the following:

    // Original handle world position at the beginning of the drag.
    m_vecOriginalHandlePos = m_pHandle->parentToRoot(m_pHandle->position());

    // Original mouse position in the viewport.
    m_BeginDragPos = e->pos();

    // Distance between camera and handle - to maintain when dragging relative to camera.
    m_flHandleCamDist = (m_pHandle->position() - v->camera()->position()).length();

    // The flags for the axes we're constrained to.
    m_iAxisFlags = UIManipulator::axisFlagsFromPickColor(col);

    // The axes themselves
    m_MovementAxes = UIManipulator::manipulationAxes(m_iAxisFlags);

    m_bInMove = true;
}

void ScaleTool::vMouseMove(QMouseEvent *e)
{
    if ( !m_bInMove )
    {
        BaseTool::vMouseMove(e);
        return;
    }

    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v || !v->camera() )
    {
        BaseTool::vMouseMove(e);
        return;
    }

    // Get the current grid multiple.
    unsigned int gridMultiple = m_pDocument->scene()->grid()->gridMultiple();

    // If there is no constraint on the movement axis, scale in all dimensions.
    if ( m_iAxisFlags == UIManipulator::AxisXYZ )
    {
        // TODO
    }

    // Otherwise, scale only in the specified axis or along the specified plane.
    else
    {
        // Get the axis identifier for our movement constraint flags.
        // This indicates the plane where this axis value is 0.
        Math::AxisIdentifier axis = UIManipulator::planeConstraintAxis(m_iAxisFlags, *v->camera());

        // Get the co-ordinate of the handle in this axis.
        float value = m_vecOriginalHandlePos[axis];

        // Generate a ray from the camera origin that passes through the initial drag position,
        // and find where it intersects the axis=value plane.
        // Eg. where x=5.
        bool success = false;
        Ray3D initialRay(v->camera()->position(), v->camera()->frustumDirection(m_BeginDragPos, v->size()));
        QVector3D initialIntersection = initialRay.parameterise(axis, value, &success);
        if ( !success )
            return;

        // Do the same for the current drag position.
        success = false;
        Ray3D planeRay(v->camera()->position(), v->camera()->frustumDirection(e->pos(), v->size()));
        QVector3D planeIntersection = planeRay.parameterise(axis, value, &success);
        if ( !success )
            return;

        // Don't clamp, because in the future we'll need to clamp to
        // scale multiples instead, not grid values.
        // UIManipulator::clampToNearestMultiple(planeIntersection, gridMultiple, m_iAxisFlags);

        // The scale is the ratio of the new vector to the original vector.
        // The vectors are anchored at the handle position.
        QVector3D originalScaleVec = initialIntersection - m_vecOriginalHandlePos;
        QVector3D newScaleVec = planeIntersection - m_vecOriginalHandlePos;

        // Ensure the scale only occurs in the axes we want.
        QVector3D constrainedOriginalVec;
        QVector3D constrainedNewVec;
        foreach ( QVector3D axis, m_MovementAxes )
        {
            constrainedOriginalVec += QVector3D::dotProduct(originalScaleVec, axis) * axis;
            constrainedNewVec += QVector3D::dotProduct(newScaleVec, axis) * axis;
        }

        m_vecScale = QVector3D(1,1,1);

        for ( int i = 0; i < 3; i++ )
        {
            if ( qFuzzyIsNull(constrainedOriginalVec[i]) )
                continue;

            float sc = constrainedNewVec[i] / constrainedOriginalVec[i];
            if ( sc < 0.0f )
                sc = 0.0f;

            m_vecScale[i] = sc;
        }

        qDebug() << "Scale:" << m_vecScale;
    }

    m_pHandle->rescaleHandle(m_vecScale);
    updateTableManipulators();
}

void ScaleTool::vMouseRelease(QMouseEvent *e)
{
    if ( !m_bInMove )
    {
        BaseTool::vMouseRelease(e);
        return;
    }

    endMove();
}

void ScaleTool::endMove()
{
    m_pHandle->rescaleHandle(QVector3D(1,1,1));
    commitTableManipulators();
    m_bInMove = false;
}

void ScaleTool::updateTableFromSet()
{
    typedef QSet<SceneObject*> SOSet;
    const SOSet &set = m_pDocument->selectedSet();

    if ( set.count() < 1 )
    {
        clearTableManipulators();
        return;
    }

    // Remove our manipulators that are no longer present in the set.
    QList<SceneObject*> toRemove;
    for ( ManipTable::const_iterator it = m_ManipTable.constBegin(); it != m_ManipTable.constEnd(); ++it )
    {
        if ( !set.contains(it.key()) )
            toRemove.append(it.key());
    }

    foreach ( SceneObject* o, toRemove )
    {
        m_ManipTable.remove(o);
    }

    // Add new items that are not present in our table.
    for ( SOSet::const_iterator it = set.constBegin(); it != set.constEnd(); ++it )
    {
        if ( !m_ManipTable.contains(*it) )
        {
            m_ManipTable.insert((*it), SceneObjectManipulator(*it));
        }
    }
}

void ScaleTool::updateTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        SceneObject* obj = it.key();
        SceneObjectManipulator& m = it.value();

        // If the object already has ancestors in the table, don't set its position - it will inherit.
        if ( isAncestorInManipulatorTable(obj) )
            continue;

        m.setScale(m_vecScale);
    }
}

void ScaleTool::commitTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        SceneObject* obj = it.key();
        SceneObjectManipulator& m = it.value();

        // If the object already has ancestors in the table, don't do anything - it will inherit.
        if ( isAncestorInManipulatorTable(obj) )
            continue;

        // We want to visualise scales of 0 when editing, but don't want to apply them.
        // Therefore make sure there is no scale of 0 here.
        QVector3D scale = m.scale();
        for ( int i = 0; i < 3; i++ )
        {
            if ( qFuzzyIsNull(scale[i]) )
                scale[i] = 1;
        }
        m.setScale(scale);

        qDebug() << "Applying" << obj << "manipulator scale:" << scale;
        m.apply();
    }
}

bool ScaleTool::isAncestorInManipulatorTable(const SceneObject *obj) const
{
    SceneObject* p = obj->parentObject();
    while (p)
    {
        if ( m_ManipTable.contains(p) )
            return true;

        p = p->parentObject();
    }

    return false;
}

void ScaleTool::clearTableManipulators()
{
    m_ManipTable.clear();
}

void ScaleTool::updateHandleState()
{
    Q_ASSERT(m_pHandle);
    const QSet<SceneObject*> &sset = m_pDocument->selectedSet();
    m_pHandle->setPosition(sset.count() == 1 ? (*sset.begin())->localToRoot(QVector3D(0,0,0)) : m_pDocument->selectedSetCentroid());
    m_pHandle->setHidden(sset.count() < 1);
}
