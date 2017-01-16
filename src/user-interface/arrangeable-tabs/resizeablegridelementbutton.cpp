#include "resizeablegridelementbutton.h"
#include <QMouseEvent>
#include <QGuiApplication>
#include <QtDebug>

namespace UserInterface
{
    ResizeableGridElementButton::ResizeableGridElementButton(const ResizeModeFlags &flags, QWidget *parent)
        : QPushButton(parent),
          m_iResizeFlags(flags),
          m_iRowIndex(-1),
          m_iColumnIndex(-1)
    {
        calculateSizePolicy();
        setCustomPalette();
        connect(this, SIGNAL(released()), this, SLOT(buttonReleased()));
    }

    void ResizeableGridElementButton::mouseMoveEvent(QMouseEvent *e)
    {
        if ( !m_pLastMousePos )
        {
            m_pLastMousePos.reset(new QPoint(e->globalPos()));
            return;
        }

        QPoint newPos = e->globalPos();
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

    int ResizeableGridElementButton::rowIndex() const
    {
        return m_iRowIndex;
    }

    void ResizeableGridElementButton::setRowIndex(int index)
    {
        m_iRowIndex = index;
    }

    int ResizeableGridElementButton::columnIndex() const
    {
        return m_iColumnIndex;
    }

    void ResizeableGridElementButton::setColumnIndex(int index)
    {
        m_iColumnIndex = index;
    }

    void ResizeableGridElementButton::calculateSizePolicy()
    {
        QSizePolicy::Policy hPolicy = QSizePolicy::Expanding;
        QSizePolicy::Policy vPolicy = QSizePolicy::Expanding;

        if ( m_iResizeFlags.testFlag(HorizontalResizeFlag) )
        {
            hPolicy = QSizePolicy::Fixed;
        }

        if ( m_iResizeFlags.testFlag(VerticalResizeFlag) )
        {
            vPolicy = QSizePolicy::Fixed;
        }

        setSizePolicy(hPolicy, vPolicy);
    }

    QSize ResizeableGridElementButton::sizeHint() const
    {
        return QSize(12,12);
    }

    void ResizeableGridElementButton::enterEvent(QEvent *event)
    {
        QPushButton::enterEvent(event);

        if ( m_iResizeFlags == NoResizeFlag )
            return;

        Qt::CursorShape cursor = Qt::SizeAllCursor;
        if ( !m_iResizeFlags.testFlag(HorizontalResizeFlag) )
        {
            cursor = Qt::SizeVerCursor;
        }
        else if ( !m_iResizeFlags.testFlag(VerticalResizeFlag) )
        {
            cursor = Qt::SizeHorCursor;
        }

        QGuiApplication::setOverrideCursor(QCursor(cursor));
    }

    void ResizeableGridElementButton::leaveEvent(QEvent *event)
    {
        QPushButton::enterEvent(event);

        QGuiApplication::restoreOverrideCursor();

        // If we drag off the button and let go of the mouse,
        // we get a leave event instead of a mouse released signal.
        if ( m_pLastMousePos )
            buttonReleased();
    }

    ResizeableGridElementButton::ResizeModeFlags ResizeableGridElementButton::resizeFlags() const
    {
        return m_iResizeFlags;
    }

    void ResizeableGridElementButton::setCustomPalette()
    {
        setFlat(true);
        setAutoFillBackground(true);
    }
}
