#include "keysignalsender.h"

namespace NS_MODEL
{
    KeySignalSender::KeySignalSender(QObject *parent) : QObject(parent)
    {

    }

    void KeySignalSender::triggerKeyEvent(Qt::Key key, bool pressed)
    {
        emit keyEvent(pressed, key);
    }
}
