#ifndef KEYSIGNALSENDER_H
#define KEYSIGNALSENDER_H

#include "model_global.h"
#include <QObject>
#include <QHash>

namespace NS_MODEL
{
    class MODELSHARED_EXPORT KeySignalSender : public QObject
    {
        Q_OBJECT
    public:
        explicit KeySignalSender(QObject *parent = 0);

    signals:
        void keyEvent(bool pressed, int key);

    public slots:
        void triggerKeyEvent(int key, bool pressed);
    };
}

#endif // KEYSIGNALSENDER_H
