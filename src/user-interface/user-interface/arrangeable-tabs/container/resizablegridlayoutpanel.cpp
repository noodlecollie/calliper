#include <QGridLayout>

#include "resizablegridlayoutpanel.h"
#include "user-interface/arrangeable-tabs/grid/resizeablegridlayoutmanager.h"

namespace UserInterface
{
    ResizableGridLayoutPanel::ResizableGridLayoutPanel(QWidget *parent) : QWidget(parent)
    {
        m_pGridLayout = new QGridLayout();
        m_pGridLayout->setContentsMargins(0,0,0,0);
        m_pGridLayout->setSpacing(0);
        setLayout(m_pGridLayout);

        m_pGridManager = new ResizeableGridLayoutManager(m_pGridLayout);
        connect(m_pGridManager, SIGNAL(widgetFloated(QWidget*,bool)), this, SLOT(handleWidgetFloated(QWidget*,bool)));
    }

    void ResizableGridLayoutPanel::addWidget(QWidget *widget, QuadGridLayoutDefs::GridCell cell, Qt::Orientation splitPreference)
    {
        m_pGridManager->addWidget(widget, cell, splitPreference);
    }

    void ResizableGridLayoutPanel::insertWidget(QWidget *widget, QuadGridLayoutDefs::GridCell cell)
    {
        m_pGridManager->insertWidget(widget, cell);
    }

    void ResizableGridLayoutPanel::handleWidgetFloated(QWidget *widget, bool dragged)
    {
        ResizableGridLayoutPanel* newPanel = new ResizableGridLayoutPanel();
        newPanel->addWidget(widget, QuadGridLayoutDefs::NorthWest);
        newPanel->show();

        if ( !dragged )
            return;
    }
}
