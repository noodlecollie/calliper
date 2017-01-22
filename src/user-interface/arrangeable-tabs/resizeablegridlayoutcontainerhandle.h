#ifndef RESIZEABLEGRIDLAYOUTCONTAINERHANDLE_H
#define RESIZEABLEGRIDLAYOUTCONTAINERHANDLE_H

#include "user-interface_global.h"
#include <QFrame>

namespace UserInterface
{
    class ResizeableGridLayoutContainerHandle : public QFrame
    {
        Q_OBJECT
    public:
        ResizeableGridLayoutContainerHandle(QWidget* parent = 0, Qt::WindowFlags f = 0);

        virtual QSize sizeHint() const override;
    };
}

#endif // RESIZEABLEGRIDLAYOUTCONTAINERHANDLE_H
