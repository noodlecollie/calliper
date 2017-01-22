#include "resizeablegridlayoutcontainerhandle.h"
#include <QApplication>

namespace
{
    const int SIZE = 9;
}

namespace UserInterface
{
    ResizeableGridLayoutContainerHandle::ResizeableGridLayoutContainerHandle(QWidget *parent, Qt::WindowFlags f)
        : QFrame(parent, f)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        QPalette p = QApplication::palette();
        p.setBrush(QPalette::Window, p.midlight());
        setPalette(p);

        setAutoFillBackground(true);
    }

    QSize ResizeableGridLayoutContainerHandle::sizeHint() const
    {
        return QSize(SIZE, SIZE);
    }
}
