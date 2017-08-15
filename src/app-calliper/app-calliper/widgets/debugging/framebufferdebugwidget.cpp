#include "framebufferdebugwidget.h"

#include "rendersystem/endpoints/framebufferstoreendpoint.h"

FrameBufferDebugWidget::FrameBufferDebugWidget(QWidget *parent)
    : QTableView(parent)
{
    setModel(RenderSystem::FrameBufferStoreEndpoint::constFrameBufferStore()->itemModel());
}
