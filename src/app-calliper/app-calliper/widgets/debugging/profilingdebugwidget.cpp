#include "profilingdebugwidget.h"
#include "ui_profilingdebugwidget.h"

#include "rendersystem/endpoints/rendermodelstoreendpoint.h"

ProfilingDebugWidget::ProfilingDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfilingDebugWidget)
{
    ui->setupUi(this);

    ui->treeView->setModel(RenderSystem::RenderModelStoreEndpoint::constRenderModelStore()->profilingItemModel());
}

ProfilingDebugWidget::~ProfilingDebugWidget()
{
    delete ui;
}

void ProfilingDebugWidget::onRefreshPressed()
{
    // TODO
}
