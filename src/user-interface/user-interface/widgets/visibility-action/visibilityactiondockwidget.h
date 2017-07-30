#ifndef VISIBILITYACTIONDOCKWIDGET_H
#define VISIBILITYACTIONDOCKWIDGET_H

#include "user-interface_global.h"

#include <QDockWidget>

#include "user-interface/signal-handlers/visibilityactionhandler.h"

namespace UserInterface
{
    class USERINTERFACESHARED_EXPORT VisibilityActionDockWidget : public QDockWidget
    {
        Q_OBJECT
    public:
        VisibilityActionDockWidget(const QString &title, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
        VisibilityActionDockWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());

        VisibilityActionHandler* visibilityActionHandler();

    private:
        VisibilityActionHandler* m_pVisibilityActionHandler;
    };
}

#endif // VISIBILITYACTIONDOCKWIDGET_H
