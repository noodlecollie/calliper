#include "inputprocessor.h"
#include "mapdocument.h"
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include "scene.h"
#include "viewport.h"
#include "basetool.h"

InputProcessor::InputProcessor(MapDocument *document) : QObject(document)
{
}

MapDocument* InputProcessor::document() const
{
    return qobject_cast<MapDocument*>(parent());
}

bool InputProcessor::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    BaseTool* tool = document()->activeTool();

    switch (event->type())
    {
        case QEvent::KeyPress:
            if ( tool )
                tool->keyPressEvent(dynamic_cast<QKeyEvent*>(event));
            return true;

        case QEvent::KeyRelease:
            if ( tool )
                tool->keyReleaseEvent(dynamic_cast<QKeyEvent*>(event));
            return true;

        case QEvent::MouseButtonPress:
            if ( tool )
                tool->mousePressEvent(dynamic_cast<QMouseEvent*>(event));
            return true;

        case QEvent::MouseMove:
            if ( tool )
                tool->mouseMoveEvent(dynamic_cast<QMouseEvent*>(event));
            return true;

        case QEvent::MouseButtonRelease:
            if ( tool )
                tool->mouseReleaseEvent(dynamic_cast<QMouseEvent*>(event));
            return true;

        case QEvent::Wheel:
            if ( tool )
                tool->wheelEvent(dynamic_cast<QWheelEvent*>(event));
            return true;

        default:
            return false;
    }
}

bool InputProcessor::filterKeyPress(QObject* watched, QKeyEvent *e)
{
    Q_UNUSED(watched);
    Q_UNUSED(e);
    return true;
}

bool InputProcessor::filterKeyRelease(QObject* watched, QKeyEvent *e)
{
    Q_UNUSED(e);
    Q_UNUSED(watched);
    return true;
}

bool InputProcessor::filterMousePress(QObject* watched, QMouseEvent *e)
{
    /*if ( e->button() == Qt::LeftButton )
    {
        setSelectionToObjectAtPixel(qobject_cast<Viewport*>(watched), e->pos());
        return true;
    }*/

    return true;
}

bool InputProcessor::filterMouseMove(QObject* watched, QMouseEvent *e)
{
    Q_UNUSED(e);
    Q_UNUSED(watched);
    return true;
}

bool InputProcessor::filterMouseRelease(QObject* watched, QMouseEvent *e)
{
    Q_UNUSED(e);
    Q_UNUSED(watched);
    return true;
}

bool InputProcessor::filterWheel(QObject* watched, QWheelEvent *e)
{
    Q_UNUSED(e);
    Q_UNUSED(watched);
    return true;
}

void InputProcessor::setSelectionToObjectAtPixel(Viewport *viewport, const QPoint &pos)
{
    Q_ASSERT(viewport);
    SceneObject* picked = viewport->pickObjectFromDepthBuffer(pos);
    MapDocument* doc = document();
    doc->selectedSetClear();

    if ( picked )
        doc->selectedSetInsert(picked);
}
