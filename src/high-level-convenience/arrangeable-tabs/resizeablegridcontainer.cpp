#include "resizeablegridcontainer.h"
#include <QGridLayout>

namespace HighLevelConvenience
{
    ResizeableGridContainer::ResizeableGridContainer(QWidget *parent)
        : QWidget(parent)
    {
        QGridLayout* gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        setLayout(gridLayout);
    }
}
