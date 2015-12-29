#include "viewportuseroptions.h"
#include "ui_viewportuseroptions.h"
#include "viewport.h"
#include <QFocusEvent>

ViewportUserOptions::ViewportUserOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewportUserOptions)
{
    ui->setupUi(this);

    Viewport* v = qobject_cast<Viewport*>(parent);
    Q_ASSERT(v);

    ui->cbFocusHighlight->setChecked(v->drawFocusHighlight());

    connect(ui->cbFocusHighlight, SIGNAL(clicked(bool)), this, SIGNAL(focusHighlightStatusChanged(bool)));
}

ViewportUserOptions::~ViewportUserOptions()
{
    delete ui;
}

void ViewportUserOptions::toggleVisibility()
{
    setVisible(!isVisible());
    if ( isVisible() )
        setFocus();
}
