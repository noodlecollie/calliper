#ifndef DEBUGTOPLEVELWIDGET_H
#define DEBUGTOPLEVELWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>

#include "user-interface/widgets/mainwindowslavewidget.h"
#include "user-interface/signal-handlers/visibilityactionhandler.h"

#include "app-calliper/widgets/debugging/framebufferdebugwidget.h"
#include "app-calliper/widgets/debugging/rendermodeldebugwidget.h"
#include "app-calliper/widgets/debugging/profilingdebugwidget.h"

class DebugTopLevelWidget : public UserInterface::MainWindowSlaveWidget
{
    Q_OBJECT
public:
    explicit DebugTopLevelWidget(QWidget *parent = 0);

    QSize sizeHint() const;
    UserInterface::VisibilityActionHandler* visibilityActionHandler();

private:
    QVBoxLayout* m_pLayout;

    QTabWidget* m_pTabWidget;
    FrameBufferDebugWidget* m_pFrameBufferDebugWidget;
    RenderModelDebugWidget* m_pRenderModelDebugWidget;
    ProfilingDebugWidget* m_pProfilingDebugWidget;

    UserInterface::VisibilityActionHandler* m_pVisibilityActionHandler;
};

#endif // DEBUGTOPLEVELWIDGET_H