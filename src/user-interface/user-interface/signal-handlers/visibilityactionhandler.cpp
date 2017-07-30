#include "visibilityactionhandler.h"

#include <QWidget>
#include <QDockWidget>
#include <QEvent>

namespace UserInterface
{
    VisibilityActionHandler::VisibilityActionHandler(QWidget* parent)
        : QObject(parent),
          m_pParentWidget(parent),
          m_pAction()
    {
        if ( !m_pParentWidget )
        {
            Q_ASSERT(false);
            return;
        }

        QDockWidget* dockWidget = qobject_cast<QDockWidget*>(m_pParentWidget);
        if ( dockWidget )
        {
            connect(dockWidget, &QDockWidget::visibilityChanged, this, &VisibilityActionHandler::handleVisibilityChanged);
            return;
        }

        // Not a dock widget, so we have to do the filtering ourselves.
        m_pParentWidget->installEventFilter(this);
    }

    QAction* VisibilityActionHandler::visibilityAction() const
    {
        return m_pAction.data();
    }

    void VisibilityActionHandler::setVisibilityAction(QAction *action)
    {
        manageConnections(m_pAction.data(), action);
        m_pAction = QPointer<QAction>(action);
    }

    void VisibilityActionHandler::clearVisibilityAction()
    {
        setVisibilityAction(Q_NULLPTR);
    }

    void VisibilityActionHandler::manageConnections(QAction *oldAction, QAction *newAction)
    {
        if ( oldAction )
        {
            disconnect(oldAction, &QAction::triggered, this, &VisibilityActionHandler::actionCheckedStateChanged);
        }

        if ( newAction )
        {
            connect(newAction, &QAction::triggered, this, &VisibilityActionHandler::actionCheckedStateChanged);
        }
    }

    void VisibilityActionHandler::handleVisibilityChanged(bool visible)
    {
        if ( !m_pAction )
        {
            return;
        }

        const QSignalBlocker blocker(m_pAction.data());
        Q_UNUSED(blocker);

        m_pAction->setChecked(visible);
    }

    void VisibilityActionHandler::actionCheckedStateChanged(bool checked)
    {
        const QSignalBlocker blocker(m_pParentWidget);
        Q_UNUSED(blocker);

        m_pParentWidget->setVisible(checked);
    }

    bool VisibilityActionHandler::eventFilter(QObject *watched, QEvent *event)
    {
        Q_ASSERT(watched && watched == parent());

        if ( event->type() == QEvent::Show )
        {
            handleVisibilityChanged(true);
        }
        else if ( event->type() == QEvent::Hide )
        {
            handleVisibilityChanged(false);
        }

        return QObject::eventFilter(watched, event);
    }
}
