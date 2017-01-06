#include "visibleactiondockwidget.h"
#include <QSignalBlocker>

namespace AppCalliper
{
    VisibleActionDockWidget::VisibleActionDockWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
        : QDockWidget(title, parent, flags)
    {
        init();
    }

    VisibleActionDockWidget::VisibleActionDockWidget(QWidget *parent, Qt::WindowFlags flags)
        : QDockWidget(parent, flags)
    {
        init();
    }

    void VisibleActionDockWidget::init()
    {
        connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(handleVisibilityChanged(bool)));
    }

    QAction* VisibleActionDockWidget::visibilityAction() const
    {
        return m_pAction.data();
    }

    void VisibleActionDockWidget::setVisibilityAction(QAction *action)
    {
        manageConnections(m_pAction.data(), action);
        m_pAction = QPointer<QAction>(action);
    }

    void VisibleActionDockWidget::clearVisibilityAction()
    {
        setVisibilityAction(nullptr);
    }

    void VisibleActionDockWidget::manageConnections(QAction *oldAction, QAction *newAction)
    {
        if ( oldAction )
        {
            disconnect(newAction, SIGNAL(triggered(bool)), this, SLOT(actionCheckedStateChanged(bool)));
        }

        if ( newAction )
        {
            connect(newAction, SIGNAL(triggered(bool)), this, SLOT(actionCheckedStateChanged(bool)));
        }
    }

    void VisibleActionDockWidget::handleVisibilityChanged(bool visible)
    {
        if ( !m_pAction )
            return;

        const QSignalBlocker blocker(m_pAction.data());
        Q_UNUSED(blocker);
        m_pAction->setChecked(visible);
    }

    void VisibleActionDockWidget::actionCheckedStateChanged(bool checked)
    {
        const QSignalBlocker blocker(this);
        Q_UNUSED(blocker);
        setVisible(checked);
    }
}
