#include "basedebugtoplevelwidget.h"

namespace AppCalliper
{
    BaseDebugTopLevelWidget::BaseDebugTopLevelWidget(QWidget *parent)
        : UserInterface::MainWindowSlaveWidget(parent),
          m_pVisibilityActionHandler(new UserInterface::VisibilityActionHandler(this))
    {

    }

    UserInterface::VisibilityActionHandler* BaseDebugTopLevelWidget::visibilityActionHandler()
    {
        return m_pVisibilityActionHandler;
    }
}
