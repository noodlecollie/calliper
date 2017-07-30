#include "visibilityactiondockwidget.h"

namespace UserInterface
{
    VisibilityActionDockWidget::VisibilityActionDockWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
        : QDockWidget(title, parent, flags),
          m_pVisibilityActionHandler(new VisibilityActionHandler(this))
    {

    }

    VisibilityActionDockWidget::VisibilityActionDockWidget(QWidget *parent, Qt::WindowFlags flags)
        : QDockWidget(parent, flags),
          m_pVisibilityActionHandler(new VisibilityActionHandler(this))
    {

    }

    VisibilityActionHandler* VisibilityActionDockWidget::visibilityActionHandler()
    {
        return m_pVisibilityActionHandler;
    }
}
