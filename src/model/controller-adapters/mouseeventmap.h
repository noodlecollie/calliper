#ifndef MOUSEEVENTMAP_H
#define MOUSEEVENTMAP_H

#include "model_global.h"
#include <QObject>
#include <QPoint>

class QMouseEvent;

namespace Model
{
    class MODELSHARED_EXPORT MouseEventMap : public QObject
    {
        Q_OBJECT
    public:
        explicit MouseEventMap(QObject *parent = 0);

        bool enabled() const;
        void setEnabled(bool enabled);

        float horizontalSensitivity() const;
        void setHorizontalSensitivity(float val);

        float verticalSensitivity() const;
        void setVerticalSensitivity(float val);

        void setLastMousePositionInvalid();

        virtual bool eventFilter(QObject *watched, QEvent *event);

    signals:
        void mouseMovedX(float val);
        void mouseMovedY(float val);

    public slots:

    private:
        void processMouseMove(QMouseEvent* e);
        void setLastMousePosition(const QPoint &p);

        bool m_bEnabled;
        float m_flHorizontalSensitivity;
        float m_flVerticalSensitivity;

        QPoint m_LastMousePosition;
        bool m_bLastMousePositionValid;
    };
}

#endif // MOUSEEVENTMAP_H
