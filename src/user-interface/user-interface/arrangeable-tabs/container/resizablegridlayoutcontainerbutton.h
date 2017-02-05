#ifndef RESIZABLEGRIDLAYOUTCONTAINERBUTTON_H
#define RESIZABLEGRIDLAYOUTCONTAINERBUTTON_H

#include <QFrame>

class QAction;
class QContextMenuEvent;
class QMouseEvent;
class QLabel;

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

        QString label() const;
        void setLabel(const QString& text);

        int dragActivationThreshold() const;
        void setDragActivationThreshold(int threshold);

    signals:
        void selectInvoked(int itemId);
        void maximiseInvoked(int itemId);
        void closeInvoked(int itemId);
        void floatInvoked(int itemId, bool dragged);

    public slots:

    protected:
        void contextMenuEvent(QContextMenuEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        void initLayout();
        void initActions();

        QAction* m_pSelectAction;
        QAction* m_pMaximiseAction;
        QAction* m_pFloatAction;
        QAction* m_pCloseAction;

        int m_iItemID;
        QLabel* m_pLabel;
        int m_iDragActivationThreshold;
        bool m_bInDrag;
        QPoint m_LastMousePress;
    };
}

#endif // RESIZABLEGRIDLAYOUTCONTAINERBUTTON_H
