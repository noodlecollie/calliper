#ifndef RESIZABLEGRIDLAYOUTCONTAINERBUTTON_H
#define RESIZABLEGRIDLAYOUTCONTAINERBUTTON_H

#include <QFrame>

class QAction;
class QContextMenuEvent;
class QMouseEvent;

#include "user-interface_global.h"

namespace UserInterface
{
    class USERINTERFACESHARED_EXPORT ResizableGridLayoutContainerButton : public QFrame
    {
        Q_OBJECT
    public:
        explicit ResizableGridLayoutContainerButton(QWidget *parent = 0);

        int itemId() const;
        void setItemId(int id);

    signals:
        void selectInvoked(int itemId);
        void maximiseInvoked(int itemId);
        void closeInvoked(int itemId);

    public slots:

    protected:
        void contextMenuEvent(QContextMenuEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;

    private:
        void initActions();

        QAction* m_pSelectAction;
        QAction* m_pMaximiseAction;
        QAction* m_pCloseAction;

        int m_iItemID;
    };
}

#endif // RESIZABLEGRIDLAYOUTCONTAINERBUTTON_H
