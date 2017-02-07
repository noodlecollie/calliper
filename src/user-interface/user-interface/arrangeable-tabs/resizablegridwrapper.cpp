#include "resizablegridwrapper.h"
#include <QGridLayout>
#include "resizeablegridlayoutmanager.h"

namespace UserInterface
{
    ResizableGridWrapper::ResizableGridWrapper(QWidget *parent)
        : QWidget(parent),
          m_pGridLayout(new QGridLayout()),
          m_pLayoutManager(new ResizeableGridLayoutManager(m_pGridLayout))
    {
        setLayout(m_pGridLayout);
        connect(m_pLayoutManager, SIGNAL(widgetFloatClicked(QWidget*)), this, SLOT(widgetFloated(QWidget*)));
    }

    void ResizableGridWrapper::widgetFloated(QWidget *widget)
    {
        /*
        if ( !widget || !m_pLayoutManager->containsWidget(widget) )
            return;

        QPoint pos = widget->mapToGlobal(QPoint(0,0));
        QSize size = widget->size();

        ResizableGridWrapper* newWrapper = new ResizableGridWrapper();
        newWrapper->m_pLayoutManager->insertWidget(widget, QuadGridLayoutDefs::NorthWest, Qt::Horizontal);

        newWrapper->move(pos);
        newWrapper->resize(size);
        newWrapper->show();
        */
    }
}
