#include <QGridLayout>
#include "quadgridwidget.h"

namespace UserInterface
{
    QuadGridWidget::QuadGridWidget(QWidget *parent) : QWidget(parent),
        m_pGridManager(Q_NULLPTR)
    {
        QGridLayout* gridLayout = new QGridLayout();
        gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setSpacing(0);

        setLayout(gridLayout);

        m_pGridManager = new UserInterface::ResizeableGridLayoutManager(gridLayout);
    }

    QuadGridWidget::~QuadGridWidget()
    {
        delete m_pGridManager;
        m_pGridManager = Q_NULLPTR;
    }

    void QuadGridWidget::clear()
    {
        m_pGridManager->clear();
    }

    void QuadGridWidget::setSingleWidget(QWidget *widget)
    {
        if ( !widget )
        {
            return;
        }

        clear();

        // None of these params apart from the widget actually matter.
        m_pGridManager->addWidget(widget, UserInterface::QuadGridLayoutDefs::NorthWest, Qt::Horizontal);
    }
}
