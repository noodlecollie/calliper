#include "inputprocessor.h"
#include "mapdocument.h"
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include "scene.h"
#include "basegrid.h"
#include "viewport.h"

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

    switch (event->type())
    {
        case QEvent::KeyPress:
            return filterKeyPress(watched, dynamic_cast<QKeyEvent*>(event));

        case QEvent::KeyRelease:
            return filterKeyRelease(watched, dynamic_cast<QKeyEvent*>(event));

        case QEvent::MouseButtonPress:
            return filterMousePress(watched, dynamic_cast<QMouseEvent*>(event));

        case QEvent::MouseMove:
            return filterMouseMove(watched, dynamic_cast<QMouseEvent*>(event));

        case QEvent::MouseButtonRelease:
            return filterMouseRelease(watched, dynamic_cast<QMouseEvent*>(event));

        case QEvent::Wheel:
            return filterWheel(watched, dynamic_cast<QWheelEvent*>(event));

        default:
            return false;
    }
}

bool InputProcessor::filterKeyPress(QObject* watched, QKeyEvent *e)
{
    Q_UNUSED(watched);

    if ( e->isAutoRepeat() )
        return true;

    MapDocument* doc = document();

    switch (e->key())
    {
        case Qt::Key_BracketLeft:
        {
            doc->scene()->grid()->decrementGridPower();
            return true;
        }

        case Qt::Key_BracketRight:
        {
            doc->scene()->grid()->incrementGridPower();
            return true;
        }

        default:
            return true;
    }
}

bool InputProcessor::filterKeyRelease(QObject* watched, QKeyEvent *e)
{
    Q_UNUSED(e);
    Q_UNUSED(watched);
    return true;
}

bool InputProcessor::filterMousePress(QObject* watched, QMouseEvent *e)
{
    if ( e->button() == Qt::LeftButton )
    {
        setSelectionToObjectAtPixel(qobject_cast<Viewport*>(watched), e->pos());
        return true;
    }

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
