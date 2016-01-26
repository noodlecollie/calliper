#include "inputprocessor.h"
#include "mapdocument.h"
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include "mapscene.h"
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
