#include "changenotifier.h"

namespace Containers
{
    ChangeNotifier::ChangeNotifier(QObject *parent)
        : QObject(parent)
    {

    }

    void ChangeNotifier::notifyStoreChanged()
    {
        emit storeChanged();
    }
}
