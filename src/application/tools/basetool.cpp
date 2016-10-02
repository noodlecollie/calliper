#include "basetool.h"
#include "mapdocument.h"
#include "mapscene.h"
#include "mapgrid.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEnterEvent>
#include "viewport.h"
#include "application.h"
#include "mainwindow.h"
#include "scenecamera.h"
#include "callipermath.h"
#include <QCursor>
#include "uiscene.h"
#include <QStandardPaths>
#include "viewport.h"

BaseTool::BaseTool(const QString &name, MapDocument *document) : QObject(nullptr)
{
    setObjectName(name);
    m_bActive = false;
    m_pDocument = document;
    Q_ASSERT(m_pDocument);

    m_flKBModifiers = 0;
    m_CameraController.setAccelTime(0.1f);
    m_CameraController.setDecelTime(0.1f);
    m_CameraController.setTopSpeed(1200.0f);
    m_bMouseLookEnabled = false;
    m_bMouseEntered = false;
}

BaseTool::~BaseTool()
{
    Q_ASSERT(!m_bActive);
}

void BaseTool::activate()
{
    if ( m_bActive )
        return;

    vActivate();
    m_bActive = true;

    // If the mouse is within the viewport, generate an enter event.
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v )
        return;

    if ( isMouseWithinViewport(v) )
    {
        QEnterEvent e = generateEnterEvent(v);
        enterEvent(&e);
    }
}

void BaseTool::deactivate()
{
    if ( !m_bActive )
        return;

    // If the mouse has entered, send a leave event.
    if ( m_bMouseEntered )
    {
        QEvent e(QEvent::Leave);
        leaveEvent(&e);
    }

    vDeactivate();
    m_bActive = false;
}

void BaseTool::mousePressEvent(QMouseEvent *e)
{
    Q_ASSERT(m_bActive);
    vMousePress(e);
}

void BaseTool::mouseMoveEvent(QMouseEvent *e)
{
    Q_ASSERT(m_bActive);
    if ( e->buttons() == Qt::NoButton )
        vMouseMoveHover(e);
    else
        vMouseMove(e);
}

void BaseTool::mouseReleaseEvent(QMouseEvent *e)
{
    Q_ASSERT(m_bActive);
    vMouseRelease(e);
}

void BaseTool::wheelEvent(QWheelEvent *e)
{
    Q_ASSERT(m_bActive);
    vWheel(e);
}

void BaseTool::keyPressEvent(QKeyEvent *e)
{
    Q_ASSERT(m_bActive);
    vKeyPress(e);
}

void BaseTool::keyReleaseEvent(QKeyEvent *e)
{
    Q_ASSERT(m_bActive);
    vKeyRelease(e);
}

void BaseTool::enterEvent(QEnterEvent *e)
{
    Q_ASSERT(m_bActive);
    m_bMouseEntered = true;
    vEnter(e);
}

void BaseTool::leaveEvent(QEvent *e)
{
    Q_ASSERT(m_bActive);
    vLeave(e);
    m_bMouseEntered = false;
}

bool BaseTool::isActive() const
{
    return m_bActive;
}

QString BaseTool::toolName() const
{
    return QString("INVALID_TOOL_NAME");
}

MapDocument* BaseTool::document() const
{
    return m_pDocument;
}

void BaseTool::selectedSetChanged()
{
    Q_ASSERT(m_bActive);
    vSelectedSetChanged();
}

void BaseTool::vActivate()
{
}

void BaseTool::vDeactivate()
{
    m_flKBModifiers = 0;
    m_CameraController.clearMovementFlags();
    setMouseLookEnabled(false);
}

void BaseTool::vMousePress(QMouseEvent *e)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v )
        return;

    SceneObject* obj = v->pickObjectFromDepthBuffer(BaseScene::MapSceneFlag, e->pos());
	addToSelectedSet(obj, !m_flKBModifiers.testFlag(Qt::ControlModifier));
}

void BaseTool::addToSelectedSet(SceneObject *obj, bool clearPrevious)
{
	if ( clearPrevious )
	{
		m_pDocument->selectedSetClear();
		if ( obj )
			m_pDocument->selectedSetInsert(obj);
	}
	else
	{
		if ( !obj )
			return;

		if ( m_pDocument->selectedSet().contains(obj) )
		{
			m_pDocument->selectedSetRemove(obj);
		}
		else
		{
			m_pDocument->selectedSetInsert(obj);
		}
	}
}

void BaseTool::vMouseMove(QMouseEvent *e)
{
    handleMouseLook(e);
}

void BaseTool::vMouseMoveHover(QMouseEvent *e)
{
    handleMouseLook(e);
}

void BaseTool::handleMouseLook(QMouseEvent *e)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v )
        return;

    SceneCamera* camera = v->camera();
    if ( !camera )
        return;

    if ( m_bMouseLookEnabled )
    {
        // Determine how far the cursor has moved.
        QPoint globalCentre = v->mapToGlobal(v->viewCentre());
        QPoint delta = e->globalPos() - globalCentre;

        // Change the camera angles.
        EulerAngle angles = camera->angles();
        angles.setPitch(angles.pitch() + delta.y());
        angles.setYaw(angles.yaw() - delta.x());
        camera->setAngles(angles);

        // Cache the new position.
        QCursor::setPos(v->mapToGlobal(v->viewCentre()));
    }
}

void BaseTool::vEnter(QEnterEvent *)
{
}

void BaseTool::vLeave(QEvent *)
{
}

void BaseTool::vMouseRelease(QMouseEvent *)
{
}

void BaseTool::vWheel(QWheelEvent *e)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v )
        return;

    if (  e->orientation() == Qt::Vertical )
    {
        v->camera()->translate(QVector3D(e->delta(), 0, 0));
    }
    else
    {
        v->camera()->translate(QVector3D(0, e->delta(), 0));
    }
}

void BaseTool::vKeyPress(QKeyEvent *e)
{
    if ( e->isAutoRepeat() )
        return;

    switch (e->key())
    {
        case Qt::Key_BracketLeft:
        {
            m_pDocument->scene()->grid()->decrementGridPower();
            return;
        }

        case Qt::Key_BracketRight:
        {
            m_pDocument->scene()->grid()->incrementGridPower();
            return;
        }

        case Qt::Key_Control:
        {
            m_flKBModifiers |= Qt::ControlModifier;
            return;
        }

        case Qt::Key_W:
        {
            m_CameraController.forward(true);
            return;
        }

        case Qt::Key_S:
        {
            m_CameraController.backward(true);
            return;
        }

        case Qt::Key_A:
        {
            m_CameraController.left(true);
            return;
        }

        case Qt::Key_D:
        {
            m_CameraController.right(true);
            return;
        }

        case Qt::Key_Z:
        {
            toggleMouseLookEnabled();
            return;
        }

        case Qt::Key_Delete:
        {
            deleteSelectedObjects();
            return;
        }

        default:
            return;
    }
}

void BaseTool::vKeyRelease(QKeyEvent *e)
{
    if ( e->isAutoRepeat() )
        return;

    switch (e->key())
    {
        case Qt::Key_Control:
        {
            m_flKBModifiers &= ~Qt::ControlModifier;
            return;
        }

        case Qt::Key_W:
        {
            m_CameraController.forward(false);
            return;
        }

        case Qt::Key_S:
        {
            m_CameraController.backward(false);
            return;
        }

        case Qt::Key_A:
        {
            m_CameraController.left(false);
            return;
        }

        case Qt::Key_D:
        {
            m_CameraController.right(false);
            return;
        }

        case Qt::Key_F5:
        {
            saveFrameToDesktop();
            return;
        }
    }
}

void BaseTool::vSelectedSetChanged()
{
}

Qt::KeyboardModifiers BaseTool::keyboardModifiers() const
{
    return m_flKBModifiers;
}

bool BaseTool::keyboardModifierActive(Qt::KeyboardModifier mod) const
{
    return m_flKBModifiers.testFlag(mod);
}

void BaseTool::update(int msec)
{
    Q_ASSERT(m_bActive);
    vUpdate(msec);
}

void BaseTool::vUpdate(int msec)
{
    m_CameraController.update(msec);

    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v )
        return;

    QVector3D localMovement = m_CameraController.velocity();
    float frac = (float)msec/1000.0f;
    localMovement *= frac;
    QVector3D globalMovement(0, 0, localMovement.z());
    localMovement.setZ(0);

    v->camera()->translate(localMovement);
    v->camera()->setPosition(v->camera()->position() + globalMovement);
}

void BaseTool::setMouseLookEnabled(bool enabled)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v )
        enabled = false;

    if ( enabled == m_bMouseLookEnabled || !v )
        return;

    if ( enabled )
    {
        QPoint p = v->mapToGlobal(v->viewCentre());
        QCursor::setPos(p);
        v->setCursor(Qt::BlankCursor);
    }
    else
    {
        v->setCursor(Qt::ArrowCursor);
    }

    m_bMouseLookEnabled = enabled;
}

void BaseTool::toggleMouseLookEnabled()
{
    setMouseLookEnabled(!m_bMouseLookEnabled);
}

void BaseTool::saveFrameToDesktop()
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v )
        return;

    QString path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QString("/frame.png");
    v->saveCurrentFrame(path);
}

bool BaseTool::isMouseWithinViewport(Viewport *v)
{
    return v->rect().contains(v->mapFromGlobal(QCursor::pos()));
}

QEnterEvent BaseTool::generateEnterEvent(Viewport* v)
{
    QPoint mouseGlobal = QCursor::pos();
    return QEnterEvent(v->mapFromGlobal(mouseGlobal), v->window()->mapFromGlobal(mouseGlobal), mouseGlobal);
}

void BaseTool::deleteSelectedObjects()
{
    QSet<SceneObject*> objects = m_pDocument->selectedSet();

    foreach ( SceneObject* obj, objects )
    {
        // We should check each time whether the given object is still in the selected set,
        // because it could already have been destroyed when a previous parent was destroyed.
        if ( !m_pDocument->selectedSet().contains(obj) )
            continue;

        m_pDocument->scene()->destroySceneObject(obj);
    }
}
