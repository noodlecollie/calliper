#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>

#include "resizablegridlayoutcontainerbutton.h"

namespace UserInterface
{
    ResizableGridLayoutContainerButton::ResizableGridLayoutContainerButton(QWidget *parent)
        : QFrame(parent)
    {
        initActions();
    }

    void ResizableGridLayoutContainerButton::initActions()
    {
        m_pSelectAction = new QAction(tr("&Select"));
        connect(m_pSelectAction, SIGNAL(triggered(bool)), this, SIGNAL(selectInvoked()));

        m_pMaximiseAction = new QAction(tr("&Maximise"));
        connect(m_pMaximiseAction, SIGNAL(triggered(bool)), this, SIGNAL(maximiseInvoked()));

        m_pCloseAction = new QAction(tr("&Close"));
        connect(m_pCloseAction, SIGNAL(triggered(bool)), this, SIGNAL(closeInvoked()));
    }

    void ResizableGridLayoutContainerButton::contextMenuEvent(QContextMenuEvent *event)
    {
        QMenu menu(this);

        menu.addAction(m_pSelectAction);
        menu.addAction(m_pMaximiseAction);
        menu.addAction(m_pCloseAction);

        menu.exec(event->globalPos());
    }

    void ResizableGridLayoutContainerButton::mousePressEvent(QMouseEvent *event)
    {
        switch ( event->button() )
        {
            case Qt::LeftButton:
            {
                m_pSelectAction->trigger();
                return;
            }

            case Qt::MiddleButton:
            {
                m_pCloseAction->trigger();
                return;
            }

            default:
                break;
        }

        QFrame::mousePressEvent(event);
    }

    void ResizableGridLayoutContainerButton::mouseDoubleClickEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
        m_pMaximiseAction->trigger();
    }
}
