#include "rendermodeldebugwidget.h"

#include "rendersystem/endpoints/rendermodelstoreendpoint.h"

RenderModelDebugWidget::RenderModelDebugWidget(QWidget *parent)
    : QTableView(parent)
{
    setModel(RenderSystem::RenderModelStoreEndpoint::constRenderModelStore()->itemModel());
}
