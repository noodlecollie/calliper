#ifndef VISIBILITYACTIONHANDLER_H
#define VISIBILITYACTIONHANDLER_H

#include "user-interface_global.h"

#include <QObject>
#include <QAction>
#include <QPointer>

class QWidget;

namespace UserInterface
{
    class USERINTERFACESHARED_EXPORT VisibilityActionHandler : public QObject
    {
        Q_OBJECT
    public:
        explicit VisibilityActionHandler(QWidget* parent);

        QAction* visibilityAction() const;
        void setVisibilityAction(QAction* action);
        void clearVisibilityAction();

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override;

    private slots:
        void actionCheckedStateChanged(bool checked);
        void handleVisibilityChanged(bool visible);

    private:
        void manageConnections(QAction* oldAction, QAction* newAction);

        QWidget* m_pParentWidget;
        QPointer<QAction> m_pAction;
    };
}

#endif // VISIBILITYACTIONHANDLER_H
