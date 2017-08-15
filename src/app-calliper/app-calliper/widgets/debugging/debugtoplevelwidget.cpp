#include "debugtoplevelwidget.h"

namespace AppCalliper
{
    DebugTopLevelWidget::DebugTopLevelWidget(QWidget *parent)
        : UserInterface::MainWindowSlaveWidget(parent),
          m_pLayout(new QVBoxLayout()),
          m_pTabWidget(new QTabWidget()),
          m_pFrameBufferDebugWidget(new FrameBufferDebugWidget()),
          m_pVisibilityActionHandler(new UserInterface::VisibilityActionHandler(this))
    {
        setWindowTitle(tr("Debugging"));

        m_pLayout->setMargin(0);
        setLayout(m_pLayout);

        m_pLayout->addWidget(m_pTabWidget);

        m_pTabWidget->addTab(m_pFrameBufferDebugWidget, tr("Frame Buffers"));
    }

    QSize DebugTopLevelWidget::sizeHint() const
    {
        return QSize(500, 300);
    }

    UserInterface::VisibilityActionHandler* DebugTopLevelWidget::visibilityActionHandler()
    {
        return m_pVisibilityActionHandler;
    }
}
