#ifndef RESIZEABLEGRIDELEMENTBUTTON_H
#define RESIZEABLEGRIDELEMENTBUTTON_H

#include "user-interface_global.h"
#include <QFrame>
#include <QScopedPointer>
#include <QPoint>

namespace UserInterface
{
    class ResizeableGridElementButton : public QFrame
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

        int rowIndex() const;
        void setRowIndex(int index);

        int columnIndex() const;
        void setColumnIndex(int index);

    signals:
        void mouseMoved(int deltaX, int deltaY);

    protected:
        virtual void mouseMoveEvent(QMouseEvent *e) override;
        virtual void mouseReleaseEvent(QMouseEvent *e) override;
        virtual QSize sizeHint() const override;
        virtual void enterEvent(QEvent *event) override;
        virtual void leaveEvent(QEvent *event) override;

    private:
        void calculateSizePolicy();
        void initStyle();

        ResizeModeFlags m_iResizeFlags;
        QScopedPointer<QPoint> m_pLastMousePos;
        int m_iRowIndex;
        int m_iColumnIndex;
    };


    Q_DECLARE_OPERATORS_FOR_FLAGS(ResizeableGridElementButton::ResizeModeFlags)
}

#endif // RESIZEABLEGRIDELEMENTBUTTON_H
