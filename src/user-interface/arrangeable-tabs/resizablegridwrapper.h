#ifndef RESIZABLEGRIDWRAPPER_H
#define RESIZABLEGRIDWRAPPER_H

#include "user-interface_global.h"
#include <QWidget>

class QGridLayout;

namespace UserInterface
{
    class ResizeableGridLayoutManager;

    class ResizableGridWrapper : public QWidget
    {
        Q_OBJECT
    public:
        explicit ResizableGridWrapper(QWidget *parent = 0);

    signals:

    public slots:

    private slots:
        void widgetFloated(QWidget* widget);

    private:
        QGridLayout* m_pGridLayout;
        ResizeableGridLayoutManager* m_pLayoutManager;
    };
}

#endif // RESIZABLEGRIDWRAPPER_H
