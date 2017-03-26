#ifndef KEYSIGNALSENDER_H
#define KEYSIGNALSENDER_H

#include "model_global.h"
#include <QObject>
#include <QHash>

namespace Model
{
    class MODELSHARED_EXPORT KeySignalSender : public QObject
    {
        Q_OBJECT
    public:
        enum SignalModeFlag
        {
            KeyPress = 0x1,
            KeyRelease = 0x2,
        };
        Q_DECLARE_FLAGS(SignalModeFlags, SignalModeFlag)
        Q_FLAG(SignalModeFlags)

        explicit KeySignalSender(SignalModeFlags signalMode, QObject *parent = 0);

        SignalModeFlags signalMode() const;

    signals:
        void keyEvent(bool pressed, int key);

    public slots:
        void triggerKeyEvent(int key, bool pressed);

    private:
        SignalModeFlags m_nSignalModeFlags;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(KeySignalSender::SignalModeFlags)
}

#endif // KEYSIGNALSENDER_H
