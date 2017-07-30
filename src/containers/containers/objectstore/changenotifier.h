#ifndef CHANGENOTIFIER_H
#define CHANGENOTIFIER_H

#include "containers_global.h"

#include <QObject>

namespace Containers
{
    // This is good enough for now. If we want more granularity
    // we can re-implement QAbstractItemStore's signals later.
    class CONTAINERSSHARED_EXPORT ChangeNotifier : public QObject
    {
        Q_OBJECT
    public:
        explicit ChangeNotifier(QObject *parent = 0);

    signals:
        void storeChanged();

    public slots:
        void notifyStoreChanged();
    };
}

#endif // CHANGENOTIFIER_H
