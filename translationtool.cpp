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
    updateHandleState();
    m_bInMove = false;
}

void TranslationTool::vDeactivate()
{
    if ( m_bInMove )
        endMove();

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
        QVector3D worldTranslation = v->camera()->worldTranslation(m_BeginDragPos, e->pos(), v->size(), m_flHandleCamDist);

        QVector3D newHandlePos = m_vecOriginalHandlePos + worldTranslation;
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

        // Set it on the handle.
        m_pHandle->setPosition(m_pHandle->rootToParent(newHandlePos));
    }
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
    BaseTool::vSelectedSetChanged();
}

void TranslationTool::endMove()
{
    m_bInMove = false;
}

void TranslationTool::updateHandleState()
{
    Q_ASSERT(m_pHandle);
    m_pHandle->setPosition(m_pDocument->selectedSetCentroid());
    m_pHandle->setHidden(m_pDocument->selectedSet().count() < 1);
}

void TranslationTool::updateTableFromSet()
{
    updateHandleState();
}
