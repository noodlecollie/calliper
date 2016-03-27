#include "translationtool.h"
#include <QKeyEvent>
#include "translationhandle.h"
#include "mapdocument.h"
#include "uiscene.h"
#include "viewport.h"
#include "scenecamera.h"
#include "mapscene.h"
#include "ray3d.h"
#include "application.h"
#include "mainwindow.h"
#include "mapgrid.h"

TranslationTool::TranslationTool(MapDocument *document) : BaseTool(TranslationTool::staticName(), document)
{
    m_bInMove = false;
    m_pHandle = NULL;
}

TranslationTool::~TranslationTool()
{
	if ( m_bActive )
	{
		deactivate();
	}
}

QString TranslationTool::staticName()
{
	return QString("TranslationTool");
}

void TranslationTool::vActivate()
{
    m_pHandle = new TranslationHandle(m_pDocument->uiScene()->root());
    m_pHandle->setObjectName("_translationHandle");
    updateTableFromSet();
    updateHandleState();
    m_bInMove = false;
}

void TranslationTool::vDeactivate()
{
    if ( m_bInMove )
        endMove();

    clearTableManipulators();
    delete m_pHandle;
    m_pHandle = NULL;
    BaseTool::vDeactivate();
}

void TranslationTool::vKeyPress(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    BaseTool::vKeyPress(e);
}

void TranslationTool::vKeyRelease(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    BaseTool::vKeyRelease(e);
}

void TranslationTool::vMousePress(QMouseEvent *e)
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
    m_iAxisFlags = TranslationHandle::axisFlagsFromPickColor(col);

    // The axes themselves
    m_MovementAxes = UIManipulator::manipulationAxes(m_iAxisFlags);

    m_bInMove = true;
}

void TranslationTool::vMouseMove(QMouseEvent *e)
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

    // If there is no constraint on the movement axis, just move in the camera plane.
    if ( m_iAxisFlags == UIManipulator::AxisXYZ )
    {
        m_vecCurrentWorldTranslation = v->camera()->worldTranslation(m_BeginDragPos, e->pos(), v->size(), m_flHandleCamDist);

        QVector3D newHandlePos = m_vecOriginalHandlePos + m_vecCurrentWorldTranslation;
        UIManipulator::clampToNearestMultiple(newHandlePos, gridMultiple, m_iAxisFlags);
        m_pHandle->setPosition(m_pHandle->rootToParent(newHandlePos));
    }

    // Otherwise, move only in the specified axis or along the specified plane.
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

        // The translation is the delta between these two intersection positions.
        QVector3D translation = planeIntersection - initialIntersection;

        // Generate an overall translation only in the axes we want.
        QVector3D overallTranslation;
        foreach ( QVector3D axis, m_MovementAxes )
        {
            // Get the projection of our translation onto the given axis and accumulate it.
            overallTranslation += QVector3D::dotProduct(translation, axis) * axis;
        }

        // Generate the new handle position.
        QVector3D newHandlePos = m_vecOriginalHandlePos + overallTranslation;

        // Snap it to the grid.
        UIManipulator::clampToNearestMultiple(newHandlePos, gridMultiple, m_iAxisFlags);

        // Store our translation so we can update our objects.
        // This must be recalculated from before as the new handle position has been clamped.
        m_vecCurrentWorldTranslation = newHandlePos - m_vecOriginalHandlePos;

        // Set it on the handle.
        m_pHandle->setPosition(m_pHandle->rootToParent(newHandlePos));
    }

    updateTableManipulators();
}

void TranslationTool::vMouseRelease(QMouseEvent *)
{
    if ( m_bInMove )
        endMove();
}

void TranslationTool::vSelectedSetChanged()
{
    if ( m_bInMove )
        endMove();

    updateTableFromSet();
    updateHandleState();
    BaseTool::vSelectedSetChanged();
}

void TranslationTool::endMove()
{
    commitTableManipulators();
    m_bInMove = false;
}

void TranslationTool::updateHandleState()
{
    Q_ASSERT(m_pHandle);
    const QSet<SceneObject*> &sset = m_pDocument->selectedSet();
    m_pHandle->setPosition(sset.count() == 1 ? (*sset.begin())->localToRoot(QVector3D(0,0,0)) : m_pDocument->selectedSetCentroid());
    m_pHandle->setHidden(sset.count() < 1);
}

void TranslationTool::updateTableFromSet()
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

void TranslationTool::updateTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        SceneObject* obj = it.key();
        SceneObjectManipulator& m = it.value();

        // If the object already has ancestors in the table, don't set its position - it will inherit.
        if ( isAncestorInManipulatorTable(obj) )
            continue;

        m.setTranslation(m_vecCurrentWorldTranslation);
    }
}

void TranslationTool::commitTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        SceneObject* obj = it.key();
        SceneObjectManipulator& m = it.value();

        // If the object already has ancestors in the table, don't do anything - it will inherit.
        if ( isAncestorInManipulatorTable(obj) )
            continue;

        qDebug() << "Applying" << obj << "manipulator translation:" << m.translation();
        m.apply();
    }
}

bool TranslationTool::isAncestorInManipulatorTable(const SceneObject *obj) const
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

void TranslationTool::clearTableManipulators()
{
    m_ManipTable.clear();
}
