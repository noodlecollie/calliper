#ifndef FRAMEBUFFERDEBUGWIDGET_H
#define FRAMEBUFFERDEBUGWIDGET_H

#include <QTableView>

class FrameBufferDebugWidget : public QTableView
{
    Q_OBJECT
public:
    explicit FrameBufferDebugWidget(QWidget* parent = Q_NULLPTR);
};

#endif // FRAMEBUFFERDEBUGWIDGET_H
