#ifndef FRAMEBUFFERDEBUGWIDGET_H
#define FRAMEBUFFERDEBUGWIDGET_H

#include <QTableView>

namespace AppCalliper
{
    class FrameBufferDebugWidget : public QTableView
    {
        Q_OBJECT
    public:
        explicit FrameBufferDebugWidget(QWidget *parent = 0);
    };
}

#endif // FRAMEBUFFERDEBUGWIDGET_H
