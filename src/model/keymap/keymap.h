#ifndef KEYMAP_H
#define KEYMAP_H

#include "model_global.h"
#include <QObject>
#include "keysignalsender.h"

namespace NS_MODEL
{
    class KeyMap : public QObject
    {
        Q_OBJECT
    public:
        explicit KeyMap(QObject *parent = 0);

        bool eventFilter(QObject *watched, QEvent *event) override;

        KeySignalSender* keyMap(Qt::Key key) const;
        KeySignalSender* addKeyMap(Qt::Key key);
        void removeKeyMap(Qt::Key key);

    private:
        typedef QHash<Qt::Key, KeySignalSender*> KeyMapTable;

        KeyMapTable m_KeyMap;
    };
}

#endif // KEYMAP_H
