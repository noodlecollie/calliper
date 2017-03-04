#include "resizeablegridlayoutcontainermenu.h"
#include "resizeablegridlayoutcontainerbutton.h"

namespace UserInterface
{
    ResizeableGridLayoutContainerMenu::ResizeableGridLayoutContainerMenu(ResizeableGridLayoutContainerButton *button, QWidget *parent)
        : QMenu(parent),
          m_pButton(button)
    {
        m_pSelectAction = new QAction(tr("&Select"));
        connect(m_pSelectAction, &QAction::triggered, this, &ResizeableGridLayoutContainerMenu::actionTriggered);

        m_pMaximiseAction = new QAction(tr("&Maximise"));
        connect(m_pMaximiseAction, &QAction::triggered, this, &ResizeableGridLayoutContainerMenu::actionTriggered);

        m_pCloseAction = new QAction(tr("&Close"));
        connect(m_pCloseAction, &QAction::triggered, this, &ResizeableGridLayoutContainerMenu::actionTriggered);

        addAction(m_pSelectAction);
        addAction(m_pMaximiseAction);
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
        else if ( action == m_pCloseAction )
        {
            emit closeInvoked();
        }

        deleteLater();
    }
}
