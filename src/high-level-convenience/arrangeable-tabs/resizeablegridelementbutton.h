#ifndef RESIZEABLEGRIDELEMENTBUTTON_H
#define RESIZEABLEGRIDELEMENTBUTTON_H

#include "high-level-convenience_global.h"
#include <QPushButton>
#include <QScopedPointer>
#include <QPoint>

namespace HighLevelConvenience
{
    class ResizeableGridElementButton : public QPushButton
    {
        Q_OBJECT
    public:
        enum ResizeModeFlag
        {
            NoResizeFlag = 0,
            HorizontalResizeFlag = (1 << 0),
            VerticalResizeFlag = (1 << 1)
        };
        Q_DECLARE_FLAGS(ResizeModeFlags, ResizeModeFlag)

        explicit ResizeableGridElementButton(const ResizeModeFlags& flags, QWidget *parent = 0);

        ResizeModeFlags resizeFlags() const;

    signals:
        void mouseMoved(int deltaX, int deltaY);

    protected:
        virtual void mouseMoveEvent(QMouseEvent *e) override;

    private slots:
        void buttonReleased();

    private:
        ResizeModeFlags m_iResizeFlags;
        QScopedPointer<QPoint> m_pLastMousePos;
    };


    Q_DECLARE_OPERATORS_FOR_FLAGS(ResizeableGridElementButton::ResizeModeFlags)
}

#endif // RESIZEABLEGRIDELEMENTBUTTON_H
