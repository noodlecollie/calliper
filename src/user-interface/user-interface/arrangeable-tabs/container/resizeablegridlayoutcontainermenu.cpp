#include "resizeablegridlayoutcontainermenu.h"
#include "resizeablegridlayoutcontainerbutton.h"

namespace UserInterface
{
    ResizeableGridLayoutContainerMenu::ResizeableGridLayoutContainerMenu(ResizeableGridLayoutContainerButton *button, QWidget *parent)
        : QMenu(parent),
          m_pButton(button)
    {
        m_pSelectAction = new QAction(tr("&Select"));
        connect(m_pSelectAction, SIGNAL(triggered(bool)), this, SLOT(actionTriggered()));

        m_pMaximiseAction = new QAction(tr("&Maximise"));
        connect(m_pMaximiseAction, SIGNAL(triggered(bool)), this, SLOT(actionTriggered()));

        m_pFloatAction = new QAction(tr("&Float"));
        connect(m_pFloatAction, SIGNAL(triggered(bool)), this, SLOT(actionTriggered()));

        m_pCloseAction = new QAction(tr("&Close"));
        connect(m_pCloseAction, SIGNAL(triggered(bool)), this, SLOT(actionTriggered()));

        addAction(m_pSelectAction);
        addAction(m_pMaximiseAction);
        addAction(m_pFloatAction);
        addSeparator();
        addAction(m_pCloseAction);
    }

    void ResizeableGridLayoutContainerMenu::actionTriggered()
    {
        if ( !m_pButton )
        {
            deleteLater();
            return;
        }

        QAction* action = qobject_cast<QAction*>(sender());

        if ( action == m_pSelectAction )
        {
            emit selectInvoked();
        }
        else if ( action == m_pMaximiseAction )
        {
            emit maximiseInvoked();
        }
        else if ( action == m_pFloatAction )
        {
            emit floatInvoked();
        }
        else if ( action == m_pCloseAction )
        {
            emit closeInvoked();
        }

        deleteLater();
    }
}
