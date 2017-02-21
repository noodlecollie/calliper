#ifndef QUADGRIDWIDGET_H
#define QUADGRIDWIDGET_H

#include "user-interface_global.h"
#include <QWidget>
#include "user-interface/arrangeable-tabs/grid/resizeablegridlayoutmanager.h"

namespace UserInterface
{
    class USERINTERFACESHARED_EXPORT QuadGridWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit QuadGridWidget(QWidget *parent = 0);
        ~QuadGridWidget();

        void clear();
        void setSingleWidget(QWidget* widget);

    signals:

    public slots:

    private:
        UserInterface::ResizeableGridLayoutManager* m_pGridManager;
    };
}

#endif // QUADGRIDWIDGET_H
