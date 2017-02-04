#include "keysignalsender.h"

namespace Model
{
    KeySignalSender::KeySignalSender(QObject *parent) : QObject(parent)
    {

    }

    void KeySignalSender::triggerKeyEvent(int key, bool pressed)
    {
        emit keyEvent(pressed, key);
    }
}
