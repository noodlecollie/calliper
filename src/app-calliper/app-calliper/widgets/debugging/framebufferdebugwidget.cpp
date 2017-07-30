#include "framebufferdebugwidget.h"
#include "ui_framebufferdebugwidget.h"

#include "rendersystem/endpoints/framebufferstoreendpoint.h"

namespace AppCalliper
{
    FrameBufferDebugWidget::FrameBufferDebugWidget(QWidget *parent)
        : BaseDebugTopLevelWidget(parent),
        ui(new Ui::FrameBufferDebugWidget)
    {
        ui->setupUi(this);

        ui->tableView->setModel(RenderSystem::FrameBufferStoreEndpoint::constFrameBufferStore()->itemModel());
    }

    FrameBufferDebugWidget::~FrameBufferDebugWidget()
    {
        delete ui;
    }
}
