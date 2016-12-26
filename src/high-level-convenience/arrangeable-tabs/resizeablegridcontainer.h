#ifndef RESIZEABLEGRIDCONTAINER_H
#define RESIZEABLEGRIDCONTAINER_H

#include "high-level-convenience_global.h"
#include <QWidget>

namespace HighLevelConvenience
{
    class ResizeableGridContainer : public QWidget
    {
        Q_OBJECT
    public:
        explicit ResizeableGridContainer(QWidget* parent = 0);
    };
}

#endif // RESIZEABLEGRIDCONTAINER_H
