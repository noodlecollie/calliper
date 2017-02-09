#ifndef RESIZEABLEGRIDLAYOUTCONTAINERMENU_H
#define RESIZEABLEGRIDLAYOUTCONTAINERMENU_H

#include <QMenu>
#include <QPoint>

class QAction;

#include "user-interface_global.h"
#include "resizeablegridlayoutcontainerbutton.h"

namespace UserInterface
{
    class ResizeableGridLayoutContainerMenu : public QMenu
    {
        Q_OBJECT
    public:
        explicit ResizeableGridLayoutContainerMenu(ResizeableGridLayoutContainerButton* button, QWidget* parent = nullptr);

    signals:
        void selectInvoked();
        void maximiseInvoked();
        void floatInvoked();
        void closeInvoked();

    private slots:
        void actionTriggered();

    private:
        QPointer<ResizeableGridLayoutContainerButton> m_pButton;

        QAction* m_pSelectAction;
        QAction* m_pMaximiseAction;
        QAction* m_pFloatAction;
        QAction* m_pCloseAction;
    };
}

#endif // RESIZEABLEGRIDLAYOUTCONTAINERMENU_H
