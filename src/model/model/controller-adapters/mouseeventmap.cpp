#include "mouseeventmap.h"
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>

namespace
{
    inline QPoint centre(QWidget* widget)
    {
        return widget->mapToGlobal(widget->rect().center());
    }
}

namespace Model
{
    MouseEventMap::MouseEventMap(QObject *parent) : QObject(parent),
        m_bEnabled(false), m_flHorizontalSensitivity(1), m_flVerticalSensitivity(1),
        m_bLastMousePositionValid(false),
        m_bShouldResetMouse(false)
    {

    }

    bool MouseEventMap::enabled() const
    {
        return m_bEnabled;
    }

    void MouseEventMap::setEnabled(bool enabled)
    {
        if ( m_bEnabled == enabled )
            return;

        m_bEnabled = enabled;

        if ( !m_bEnabled )
            m_bLastMousePositionValid = false;
    }

    bool MouseEventMap::shouldResetMouse() const
    {
        return m_bShouldResetMouse;
    }

    void MouseEventMap::setShouldResetMouse(bool shouldReset)
    {
        m_bShouldResetMouse = shouldReset;
    }

    bool MouseEventMap::eventFilter(QObject *watched, QEvent *event)
    {
        Q_UNUSED(watched);

        if ( !m_bEnabled )
        {
            return QObject::eventFilter(watched, event);
        }

        switch ( event->type() )
        {
            case QEvent::MouseMove:
                processMouseMove(watched, static_cast<QMouseEvent*>(event));
                return true;

            default:
                return QObject::eventFilter(watched, event);
        }
    }

    void MouseEventMap::processMouseMove(QObject* watched, QMouseEvent *e)
    {
        if ( !m_bLastMousePositionValid )
        {
            setLastMousePosition(e->globalPos());
            return;
        }

        QPoint pos = e->globalPos();
        int deltaX = pos.x() - m_LastMousePosition.x();
        int deltaY = pos.y() - m_LastMousePosition.y();

        if ( deltaX != 0 )
        {
            emit mouseMovedX(deltaX * m_flHorizontalSensitivity);
        }

        if ( deltaY != 0 )
        {
            emit mouseMovedY(deltaY * m_flVerticalSensitivity);
        }

        // Done first, so that if setMouseToCentreOfWidget() fails the position is still correctly cached.
        setLastMousePosition(pos);

        if ( m_bShouldResetMouse )
        {
            setMouseToCentreOfWidget(watched);
        }
    }

    void MouseEventMap::setMouseToCentreOfWidget(QObject *watched)
    {
        QWidget* widget = qobject_cast<QWidget*>(watched);
        Q_ASSERT_X(widget, Q_FUNC_INFO, "Expected watched object to be a widget!");
        if ( !widget )
        {
            return;
        }

        QPoint centrePoint = centre(widget);
        QCursor::setPos(centrePoint);
        setLastMousePosition(centrePoint);
    }

    float MouseEventMap::horizontalSensitivity() const
    {
        return m_flHorizontalSensitivity;
    }

    void MouseEventMap::setHorizontalSensitivity(float val)
    {
        m_flHorizontalSensitivity = val;
    }

    float MouseEventMap::verticalSensitivity() const
    {
        return m_flVerticalSensitivity;
    }

    void MouseEventMap::setVerticalSensitivity(float val)
    {
        m_flVerticalSensitivity = val;
    }

    void MouseEventMap::setLastMousePositionInvalid()
    {
        m_bLastMousePositionValid = false;
    }

    void MouseEventMap::setLastMousePosition(const QPoint &p)
    {
        m_LastMousePosition = p;
        m_bLastMousePositionValid = true;
    }
}
