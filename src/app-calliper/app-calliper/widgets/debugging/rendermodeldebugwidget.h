#ifndef RENDERMODELDEBUGWIDGET_H
#define RENDERMODELDEBUGWIDGET_H

#include <QTableView>

class RenderModelDebugWidget : public QTableView
{
    Q_OBJECT
public:
    RenderModelDebugWidget(QWidget* parent = Q_NULLPTR);
};

#endif // RENDERMODELDEBUGWIDGET_H
