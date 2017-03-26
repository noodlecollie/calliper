#include "keysignalsender.h"

namespace Model
{
    KeySignalSender::KeySignalSender(SignalModeFlags signalMode, QObject *parent)
        : QObject(parent),
          m_nSignalModeFlags(signalMode)
    {

    }

    KeySignalSender::SignalModeFlags KeySignalSender::signalMode() const
    {
        return m_nSignalModeFlags;
    }

    void KeySignalSender::triggerKeyEvent(int key, bool pressed)
    {
        if ( (pressed && m_nSignalModeFlags.testFlag(KeyPress)) ||
             (!pressed && m_nSignalModeFlags.testFlag(KeyRelease)) )
        {
            emit keyEvent(pressed, key);
        }
    }
}
