#include "debugtesttool.h"
#include <QKeyEvent>
#include "mapdocument.h"
#include "sceneobjectmanipulator.h"
#include "application.h"
#include "mainwindow.h"
#include "viewport.h"
#include "uiscene.h"
#include "translationhandle.h"
#include "geometryfactory.h"
#include "scenecamera.h"
#include "mapscene.h"
#include "mapgrid.h"
#include "callipermath.h"

DebugTestTool::DebugTestTool(MapDocument *document) : BaseTool(DebugTestTool::staticName(), document)
{
    m_pHandle = NULL;
    m_bInMove = false;
}

DebugTestTool::~DebugTestTool()
{
    if ( m_bActive )
    {
        deactivate();
    }
}

QString DebugTestTool::staticName()
{
    return QString("DebugTestTool");
}

void DebugTestTool::vKeyPress(QKeyEvent *e)
{
	if ( e->isAutoRepeat() || m_bInMove )
        return;

    switch (e->key())
    {
        case Qt::Key_Right:
        {
            m_vecTranslation += QVector3D(64,0,0);
            updateTableManipulators();
            break;
        }

        case Qt::Key_Left:
        {
            m_vecTranslation -= QVector3D(64,0,0);
            updateTableManipulators();
            break;
        }

        case Qt::Key_Enter:
        {
            applyTableManipulators();
            break;
        }

        default:
            BaseTool::vKeyPress(e);
            break;
    }
}

void DebugTestTool::vSelectedSetChanged()
{
    updateTableFromSet();
    BaseTool::vSelectedSetChanged();
}

void DebugTestTool::updateTableFromSet()
{
    typedef QSet<SceneObject*> SOSet;
    const SOSet &set = m_pDocument->selectedSet();

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

    // Move the handle to the centroid.
    m_pHandle->setPosition(m_pDocument->selectedSetBounds().centroid());
}

void DebugTestTool::updateTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        it.value().setTranslation(m_vecTranslation);
    }
}

void DebugTestTool::applyTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        it.value().apply();
    }
}

void DebugTestTool::vDeactivate()
{
    delete m_pHandle;
    m_pHandle = NULL;
    m_ManipTable.clear();
    m_vecTranslation = QVector3D(0,0,0);
    BaseTool::vDeactivate();
}

void DebugTestTool::vActivate()
{
    m_pHandle = new TranslationHandle(m_pDocument->uiScene()->root());
    m_pHandle->setObjectName("translationHandle");
    m_pHandle->setPosition(QVector3D(-64,0,0));
}

void DebugTestTool::vMousePress(QMouseEvent *e)
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

    m_vecOriginalHandlePos = m_pHandle->position();
    m_BeginPos = e->pos();
    m_flHandeCamDist = (m_pHandle->position() - v->camera()->position()).length();
    m_iAxisFlags = TranslationHandle::axisFlagsFromPickColor(col);
    m_MovementAxes = UIManipulator::manipulationAxes(m_iAxisFlags);

    qDebug() << "Picked colour:" << col << "Begin position:" << m_BeginPos << "Axis:" << m_MovementAxes;
    m_bInMove = true;
}

void DebugTestTool::vMouseMove(QMouseEvent *e)
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

    unsigned int gridMultiple = m_pDocument->scene()->grid()->gridMultiple();

    // If there is no constraint on the movement axis, just move in the camera plane.
    if ( m_iAxisFlags == UIManipulator::AxisXYZ )
    {
        QVector3D translation = v->camera()->worldTranslation(m_BeginPos, e->pos(), v->size(), m_flHandeCamDist);
        QVector3D newHandlePos = m_vecOriginalHandlePos + translation;
        Math::clampToNearestMultiple(newHandlePos, gridMultiple);
        m_pHandle->setPosition(newHandlePos);
    }
    // Otherwise, move only in the specified axes and along the specified plane.
    else
    {
        Math::AxisIdentifier axis = planeConstraintAxis(m_iAxisFlags, v->camera());
        float value = m_vecOriginalHandlePos[axis];

        bool success = false;
        QVector3D initialIntersection = Math::intersectionPoint(v->camera()->position(), v->camera()->frustumDirection(m_BeginPos, v->size()), axis, value, &success);
        if ( !success )
            return;

        success = false;
        QVector3D planeIntersection = Math::intersectionPoint(v->camera()->position(), v->camera()->frustumDirection(e->pos(), v->size()), axis, value, &success);
        if ( !success )
            return;

        QVector3D translation = planeIntersection - initialIntersection;
        QList<QVector3D> translationAxes = m_pHandle->manipulationAxes(m_iAxisFlags);
        QVector3D overallTranslation;
        foreach ( QVector3D axis, translationAxes )
        {
            overallTranslation += QVector3D::dotProduct(translation, axis) * axis;
        }

        QVector3D newHandlePos = m_vecOriginalHandlePos + overallTranslation;
        Math::clampToNearestMultiple(newHandlePos, gridMultiple);
        m_pHandle->setPosition(newHandlePos);
    }

    QVector3D handleTranslation = m_pHandle->position() - m_vecOriginalHandlePos;
    qDebug() << "Handle translation:" << handleTranslation;

    m_vecTranslation = handleTranslation;
	updateTableManipulators();
}

void DebugTestTool::vMouseRelease(QMouseEvent *)
{
	if ( m_bInMove )
	{
		applyTableManipulators();
	}

    m_bInMove = false;
}

Math::AxisIdentifier DebugTestTool::planeConstraintAxis(int axisFlags, const SceneCamera *camera)
{
    switch (axisFlags)
    {
        // If the axis is single, we want to choose the plane depending on which one the camera is most perpendicular to.
        case UIManipulator::AxisX:
        {
            QVector3D dir = Math::angleToVectorSimple(camera->angles());

            // Choose the plane whose normal most closely matches the camera's view direction
            if ( qAbs(QVector3D::dotProduct(dir, QVector3D(0,1,0))) > qAbs(QVector3D::dotProduct(dir, QVector3D(0,0,1))) )
            {
                return Math::AxisY;
            }
            else
            {
                return Math::AxisZ;
            }
        }

        case UIManipulator::AxisY:
        {
            QVector3D dir = Math::angleToVectorSimple(camera->angles());

            // Choose the plane whose normal most closely matches the camera's view direction
            if ( qAbs(QVector3D::dotProduct(dir, QVector3D(1,0,0))) > qAbs(QVector3D::dotProduct(dir, QVector3D(0,0,1))) )
            {
                return Math::AxisX;
            }
            else
            {
                return Math::AxisZ;
            }
        }

        case UIManipulator::AxisZ:
        {
            QVector3D dir = Math::angleToVectorSimple(camera->angles());

            // Choose the plane whose normal most closely matches the camera's view direction
            if ( qAbs(QVector3D::dotProduct(dir, QVector3D(1,0,0))) > qAbs(QVector3D::dotProduct(dir, QVector3D(0,1,0))) )
            {
                return Math::AxisX;
            }
            else
            {
                return Math::AxisY;
            }
        }

        // Otherwise, return the axis not in the plane.
        case UIManipulator::AxisXY:
        {
            return Math::AxisZ;
        }

        case UIManipulator::AxisXZ:
        {
            return Math::AxisY;
        }

        case UIManipulator::AxisYZ:
        {
            return Math::AxisX;
        }

        default:
        {
            Q_ASSERT(false);
            return Math::AxisX;
        }
    }
}
