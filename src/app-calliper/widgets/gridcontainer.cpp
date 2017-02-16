#include <QGridLayout>
#include "gridcontainer.h"

GridContainer::GridContainer(QWidget *parent) : QWidget(parent),
    m_pGridManager(nullptr)
{
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setSpacing(0);
    setLayout(gridLayout);

    m_pGridManager = new UserInterface::ResizeableGridLayoutManager(gridLayout);
}

GridContainer::~GridContainer()
{
    delete m_pGridManager;
    m_pGridManager = nullptr;
}
