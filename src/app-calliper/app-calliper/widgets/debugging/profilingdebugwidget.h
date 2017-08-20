#ifndef PROFILINGDEBUGWIDGET_H
#define PROFILINGDEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class ProfilingDebugWidget;
}

class ProfilingDebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProfilingDebugWidget(QWidget *parent = 0);
    ~ProfilingDebugWidget();

private slots:
    void onRefreshPressed();

private:
    Ui::ProfilingDebugWidget *ui;
};

#endif // PROFILINGDEBUGWIDGET_H
