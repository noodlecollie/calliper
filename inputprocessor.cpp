#include "inputprocessor.h"
#include "mapdocument.h"
#include <QEvent>
#include <QKeyEvent>
#include "scene.h"
#include "basegrid.h"

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
            return filterKeyPress(dynamic_cast<QKeyEvent*>(event));

        case QEvent::KeyRelease:
            return filterKeyRelease(dynamic_cast<QKeyEvent*>(event));

        default:
            return false;
    }
}

bool InputProcessor::filterKeyPress(QKeyEvent *e)
{
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

bool InputProcessor::filterKeyRelease(QKeyEvent *e)
{
    Q_UNUSED(e);
    return true;
}
