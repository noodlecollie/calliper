#ifndef CHANGERECEIVER_H
#define CHANGERECEIVER_H

#include "containers_global.h"

#include <QObject>
#include <functional>

namespace Containers
{
    class CONTAINERSSHARED_EXPORT ChangeReceiver : public QObject
    {
        Q_OBJECT
    public:
        explicit ChangeReceiver(QObject *parent = 0);

        void setChangeCallback(const std::function<void()>& callback);

    signals:

    public slots:
        void changeNotificationReceived();

    private:
        std::function<void()> m_ChangeCallback;
    };
}

#endif // CHANGERECEIVER_H
