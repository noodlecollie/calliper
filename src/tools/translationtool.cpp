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

TranslationTool::TranslationTool(MapDocument *document) : UIManipulatorTool(TranslationTool::staticName(), document)
{
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

UIManipulator* TranslationTool::constructManipulator()
{
    TranslationHandle* handle = m_pDocument->uiScene()->createSceneObject<TranslationHandle>(m_pDocument->uiScene()->root());
    handle->setObjectName("_translationHandle");
    return handle;
}

void TranslationTool::updateManipulatorFromMouseMove(QMouseEvent *e)
{
    Viewport* viewport = application()->mainWindow()->activeViewport();

    // Get the current grid multiple.
    unsigned int gridMultiple = m_pDocument->scene()->grid()->gridMultiple();

    // If there is no constraint on the movement axis, just move in the camera plane.
    if ( m_iAxisFlags == Math::AxisFlagXYZ )
    {
        m_Transformation.translation =
                viewport->camera()->worldTranslation(m_BeginDragPos, e->pos(), viewport->size(), m_flHandleCamDist);

        QVector3D newHandlePos = m_ManipulatorOriginalOrientation.translation
                + m_Transformation.translation;
        Math::clampToNearestMultiple(newHandlePos, gridMultiple, m_iAxisFlags);
        m_pManipulator->setPosition(m_pManipulator->rootToParent(newHandlePos));
    }

    // Otherwise, move only in the specified axis or along the specified plane.
    else
    {
        // Get the axis identifier for our movement constraint flags.
        // This indicates the plane where this axis value is 0.
        Math::AxisIdentifier axis = UIManipulator::planeConstraintAxis(m_iAxisFlags, *viewport->camera());

        // Get the co-ordinate of the handle in this axis.
        float value = m_ManipulatorOriginalOrientation.translation[axis];

        // Generate a ray from the camera origin that passes through the initial drag position,
        // and find where it intersects the axis=value plane.
        // Eg. where x=5.
        bool success = false;
        Ray3D initialRay(viewport->camera()->position(),
                         viewport->camera()->frustumDirection(m_BeginDragPos, viewport->size()));
        QVector3D initialIntersection = initialRay.parameterise(axis, value, &success);
        if ( !success )
            return;

        // Do the same for the current drag position.
        success = false;
        Ray3D planeRay(viewport->camera()->position(), viewport->camera()->frustumDirection(e->pos(), viewport->size()));
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
        QVector3D newHandlePos = m_ManipulatorOriginalOrientation.translation + overallTranslation;

        // Snap it to the grid.
        Math::clampToNearestMultiple(newHandlePos, gridMultiple, m_iAxisFlags);

        // Store our translation so we can update our objects.
        // This must be recalculated from before as the new handle position has been clamped.
        m_Transformation.translation = newHandlePos - m_ManipulatorOriginalOrientation.translation;

        // Set it on the handle.
        m_pManipulator->setPosition(m_pManipulator->rootToParent(newHandlePos));
    }

    updateTableManipulators();
}

void TranslationTool::updateManipulatorForMouseRelease(QMouseEvent *)
{
}

void TranslationTool::updateManipulator()
{
    const QSet<SceneObject*> &sset = m_pDocument->selectedSet();
    m_pManipulator->setPosition(sset.count() == 1 ? (*sset.begin())->localToRoot(QVector3D(0,0,0)) : m_pDocument->selectedSetCentroid());
    m_pManipulator->setHidden(sset.count() < 1);
}

void TranslationTool::updateSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip)
{
    manip.setTranslation(m_Transformation.translation);
}

void TranslationTool::commitSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip)
{
    manip.apply();
}

void TranslationTool::startManipulatorHover(QRgb pickColour)
{
    TranslationHandle* handle = qobject_cast<TranslationHandle*>(m_pManipulator);
    handle->setHoverSection(pickColour);
}

void TranslationTool::endManipulatorHover()
{
    TranslationHandle* handle = qobject_cast<TranslationHandle*>(m_pManipulator);
    handle->setHoverAxis(-1);

}

void TranslationTool::updateManipulatorHover(QRgb pickColour)
{
    TranslationHandle* handle = qobject_cast<TranslationHandle*>(m_pManipulator);
    handle->setHoverSection(pickColour);
}
