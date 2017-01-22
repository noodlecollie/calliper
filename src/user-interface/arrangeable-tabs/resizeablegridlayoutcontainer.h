#ifndef RESIZEABLEGRIDLAYOUTCONTAINER_H
#define RESIZEABLEGRIDLAYOUTCONTAINER_H

#include "user-interface_global.h"
#include <QFrame>

class QVBoxLayout;

namespace UserInterface
{
    class ResizeableGridLayoutContainerHandle;

    class ResizeableGridLayoutContainer : public QFrame
    {
        Q_OBJECT
    public:
        explicit ResizeableGridLayoutContainer(QWidget* parent = 0, Qt::WindowFlags f = 0);

        QWidget* item() const;

        // Returns the old item, which will need deleting.
        QWidget* replaceItem(QWidget* newItem);

        // Deletes the old item.
        void setItem(QWidget* newItem);

        bool handleVisible() const;
        void setHandleVisible(bool visible);

    signals:
        void handleDoubleClicked();
        void closeClicked();
        void maximizeClicked();
        void floatClicked();

    private:
        QVBoxLayout* vBoxLayout() const;
        void initFrame();

        QWidget* m_pItem;
        ResizeableGridLayoutContainerHandle* m_pTopHandle;
    };
}

#endif // RESIZEABLEGRIDLAYOUTCONTAINER_H
