#include "visibilityactionwidget.h"

namespace UserInterface
{
    VisibilityActionWidget::VisibilityActionWidget(QWidget *parent)
        : QWidget(parent),
          m_pVisibilityActionHandler(new VisibilityActionHandler(this))
    {
    }

    VisibilityActionHandler* VisibilityActionWidget::visibilityActionHandler()
    {
        return m_pVisibilityActionHandler;
    }
}
