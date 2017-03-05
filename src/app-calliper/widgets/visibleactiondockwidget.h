#ifndef VISIBLEACTIONDOCKWIDGET_H
#define VISIBLEACTIONDOCKWIDGET_H

#include "app-calliper_global.h"
#include <QDockWidget>
#include <QPointer>
#include <QAction>

namespace AppCalliper
{
    class VisibleActionDockWidget : public QDockWidget
    {
        Q_OBJECT
    public:
        VisibleActionDockWidget(const QString &title, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
        VisibleActionDockWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());

        QAction* visibilityAction() const;
        void setVisibilityAction(QAction* action);
        void clearVisibilityAction();

    private slots:
        void actionCheckedStateChanged(bool checked);
        void handleVisibilityChanged(bool visible);

    private:
        void init();
        void manageConnections(QAction* oldAction, QAction* newAction);

        QPointer<QAction> m_pAction;
    };
}

#endif // VISIBLEACTIONDOCKWIDGET_H
