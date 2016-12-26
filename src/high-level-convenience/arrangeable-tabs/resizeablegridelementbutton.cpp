#include "resizeablegridelementbutton.h"
#include <QMouseEvent>

namespace HighLevelConvenience
{
    ResizeableGridElementButton::ResizeableGridElementButton(const ResizeModeFlags &flags, QWidget *parent)
        : QPushButton(parent),
          m_iResizeFlags(flags)
    {
        setText("•");
        setFlat(true);
        setAutoFillBackground(true);
    }

    void ResizeableGridElementButton::mouseMoveEvent(QMouseEvent *e)
    {
        if ( !m_pLastMousePos )
        {
            m_pLastMousePos.reset(new QPoint(mapToGlobal(e->pos())));
            return;
        }

        QPoint newPos = mapToGlobal(e->pos());
        int deltaX = 0, deltaY = 0;

        if ( m_iResizeFlags.testFlag(HorizontalResizeFlag) )
        {
            deltaX = newPos.x() - m_pLastMousePos->x();
        }

        if ( m_iResizeFlags.testFlag(VerticalResizeFlag) )
        {
            deltaY = newPos.y() - m_pLastMousePos->y();
        }

        emit mouseMoved(deltaX, deltaY);
        *(m_pLastMousePos.data()) = newPos;
    }

    void ResizeableGridElementButton::buttonReleased()
    {
        m_pLastMousePos.reset();
    }
}
