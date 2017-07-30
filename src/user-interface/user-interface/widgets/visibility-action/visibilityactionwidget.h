#ifndef VISIBILITYACTIONWIDGET_H
#define VISIBILITYACTIONWIDGET_H

#include "user-interface_global.h"

#include <QWidget>

#include "user-interface/signal-handlers/visibilityactionhandler.h"

namespace UserInterface
{
    class VisibilityActionWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit VisibilityActionWidget(QWidget *parent = 0);

        VisibilityActionHandler* visibilityActionHandler();

    private:
        VisibilityActionHandler* m_pVisibilityActionHandler;
    };
}

#endif // VISIBILITYACTIONWIDGET_H
