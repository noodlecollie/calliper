#ifndef BASEDEBUGTOPLEVELWIDGET_H
#define BASEDEBUGTOPLEVELWIDGET_H

#include <QWidget>

#include "user-interface/widgets/mainwindowslavewidget.h"
#include "user-interface/signal-handlers/visibilityactionhandler.h"

namespace AppCalliper
{
    class BaseDebugTopLevelWidget : public UserInterface::MainWindowSlaveWidget
    {
        Q_OBJECT
    public:
        explicit BaseDebugTopLevelWidget(QWidget *parent = 0);

        UserInterface::VisibilityActionHandler* visibilityActionHandler();

    private:
        UserInterface::VisibilityActionHandler* m_pVisibilityActionHandler;
    };
}

#endif // BASEDEBUGTOPLEVELWIDGET_H
