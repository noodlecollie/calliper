#ifndef RESIZABLEGRIDLAYOUTPANEL_H
#define RESIZABLEGRIDLAYOUTPANEL_H

#include "user-interface_global.h"

#include <QWidget>
#include "user-interface/arrangeable-tabs/model/quadgridlayoutdefs.h"

class QGridLayout;

namespace UserInterface
{
    class ResizeableGridLayoutManager;

    class ResizableGridLayoutPanel : public QWidget
    {
        Q_OBJECT
    public:
        explicit ResizableGridLayoutPanel(QWidget *parent = 0);

        void addWidget(QWidget* widget, QuadGridLayoutDefs::GridCell cell, Qt::Orientation splitPreference = Qt::Horizontal);
        void insertWidget(QWidget* widget, QuadGridLayoutDefs::GridCell cell);

    signals:

    public slots:

    private slots:
        void handleWidgetFloated(QWidget* widget, bool dragged);

    private:
        QGridLayout* m_pGridLayout;
        UserInterface::ResizeableGridLayoutManager* m_pGridManager;
    };
}

#endif // RESIZABLEGRIDLAYOUTPANEL_H
