#include "framebufferdebugwidget.h"

#include "rendersystem/endpoints/framebufferstoreendpoint.h"

namespace AppCalliper
{
    FrameBufferDebugWidget::FrameBufferDebugWidget(QWidget *parent)
        : QTableView(parent)
    {
        setModel(RenderSystem::FrameBufferStoreEndpoint::constFrameBufferStore()->itemModel());
    }
}
