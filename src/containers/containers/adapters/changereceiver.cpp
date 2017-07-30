#include "changereceiver.h"

namespace Containers
{
    ChangeReceiver::ChangeReceiver(QObject *parent)
        : QObject(parent)
    {

    }

    void ChangeReceiver::setChangeCallback(const std::function<void ()> &callback)
    {
        m_ChangeCallback = callback;
    }

    void ChangeReceiver::changeNotificationReceived()
    {
        m_ChangeCallback();
    }
}
