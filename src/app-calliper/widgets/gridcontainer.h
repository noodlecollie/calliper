#ifndef GRIDCONTAINER_H
#define GRIDCONTAINER_H

#include <QWidget>
#include "user-interface/arrangeable-tabs/grid/resizeablegridlayoutmanager.h"

class GridContainer : public QWidget
{
    Q_OBJECT
public:
    explicit GridContainer(QWidget *parent = 0);
    ~GridContainer();

signals:

public slots:

private:
    UserInterface::ResizeableGridLayoutManager* m_pGridManager;
};

#endif // GRIDCONTAINER_H
