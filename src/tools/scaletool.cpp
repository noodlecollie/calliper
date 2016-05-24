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

ScaleTool::ScaleTool(MapDocument *document) : UIManipulatorTool(ScaleTool::staticName(), document)
{
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

UIManipulator* ScaleTool::constructManipulator()
{
    ScaleHandle* handle = m_pDocument->uiScene()->createSceneObject<ScaleHandle>(m_pDocument->uiScene()->root());
    handle->setObjectName("_scaleHandle");
    return handle;
}

void ScaleTool::updateManipulatorFromMouseMove(QMouseEvent *e)
{
    Viewport* viewport = application()->mainWindow()->activeViewport();

    // If there is no constraint on the movement axis, scale in all dimensions.
    if ( m_iAxisFlags == Math::AxisFlagXYZ )
    {
        // TODO
    }

    // Otherwise, scale only in the specified axis or along the specified plane.
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
        Ray3D initialRay(viewport->camera()->position(), viewport->camera()->frustumDirection(m_BeginDragPos, viewport->size()));
        QVector3D initialIntersection = initialRay.parameterise(axis, value, &success);
        if ( !success )
            return;

        // Do the same for the current drag position.
        success = false;
        Ray3D planeRay(viewport->camera()->position(), viewport->camera()->frustumDirection(e->pos(), viewport->size()));
        QVector3D planeIntersection = planeRay.parameterise(axis, value, &success);
        if ( !success )
            return;

        // The scale is the ratio of the new vector to the original vector.
        // The vectors are anchored at the handle position.
        QVector3D originalScaleVec = initialIntersection - m_ManipulatorOriginalOrientation.translation;
        QVector3D newScaleVec = planeIntersection - m_ManipulatorOriginalOrientation.translation;

        // Ensure the scale only occurs in the axes we want.
        QVector3D constrainedOriginalVec;
        QVector3D constrainedNewVec;
        foreach ( QVector3D axis, m_MovementAxes )
        {
            constrainedOriginalVec += QVector3D::dotProduct(originalScaleVec, axis) * axis;
            constrainedNewVec += QVector3D::dotProduct(newScaleVec, axis) * axis;
        }

        m_Transformation.scale = QVector3D(1,1,1);

        for ( int i = 0; i < 3; i++ )
        {
            if ( qFuzzyIsNull(constrainedOriginalVec[i]) )
                continue;

            float sc = constrainedNewVec[i] / constrainedOriginalVec[i];
            if ( sc < 0.0f )
                sc = 0.0f;

            m_Transformation.scale[i] = sc;
        }
    }

    qobject_cast<ScaleHandle*>(m_pManipulator)->rescaleHandle(m_Transformation.scale);
    updateTableManipulators();
}

void ScaleTool::updateManipulatorForMouseRelease(QMouseEvent *)
{
    qobject_cast<ScaleHandle*>(m_pManipulator)->rescaleHandle(QVector3D(1,1,1));
}

void ScaleTool::updateSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip)
{
    manip.setScale(m_Transformation.scale);
}

void ScaleTool::commitSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip)
{
    // We want to visualise scales of 0 when editing, but don't want to apply them.
    // Therefore make sure there is no scale of 0 here.
    QVector3D scale = manip.scale();
    for ( int i = 0; i < 3; i++ )
    {
        if ( qFuzzyIsNull(scale[i]) )
            scale[i] = 1;
    }
    manip.setScale(scale);
    manip.apply();
}

void ScaleTool::updateManipulator()
{
    const QSet<SceneObject*> &sset = m_pDocument->selectedSet();
    m_pManipulator->setPosition(sset.count() == 1 ? (*sset.begin())->localToRoot(QVector3D(0,0,0)) : m_pDocument->selectedSetCentroid());
    m_pManipulator->setHidden(sset.count() < 1);
}

void ScaleTool::startManipulatorHover(QRgb)
{
}

void ScaleTool::endManipulatorHover()
{
}

void ScaleTool::updateManipulatorHover(QRgb)
{
}
