#include "profilingdebugwidget.h"
#include "ui_profilingdebugwidget.h"

#include "rendersystem/endpoints/rendermodelstoreendpoint.h"

ProfilingDebugWidget::ProfilingDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfilingDebugWidget)
{
    ui->setupUi(this);
}

ProfilingDebugWidget::~ProfilingDebugWidget()
{
    delete ui;
}
