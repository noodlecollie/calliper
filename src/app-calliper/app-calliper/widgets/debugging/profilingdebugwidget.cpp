#include "profilingdebugwidget.h"
#include "ui_profilingdebugwidget.h"

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

void ProfilingDebugWidget::fireRefreshPressed()
{
    emit refreshPressed();
}
