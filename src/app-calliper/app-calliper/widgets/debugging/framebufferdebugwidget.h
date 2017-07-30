#ifndef FRAMEBUFFERDEBUGWIDGET_H
#define FRAMEBUFFERDEBUGWIDGET_H

#include <QWidget>

#include "basedebugtoplevelwidget.h"

namespace Ui {
class FrameBufferDebugWidget;
}

namespace AppCalliper
{
    class FrameBufferDebugWidget : public BaseDebugTopLevelWidget
    {
        Q_OBJECT

    public:
        explicit FrameBufferDebugWidget(QWidget *parent = 0);
        ~FrameBufferDebugWidget();

    private:
        Ui::FrameBufferDebugWidget *ui;
    };
}

#endif // FRAMEBUFFERDEBUGWIDGET_H
