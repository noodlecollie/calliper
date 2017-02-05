#ifndef RESIZEABLEGRIDLAYOUTCONTAINERBUTTON_H
#define RESIZEABLEGRIDLAYOUTCONTAINERBUTTON_H

#include <QFrame>

class QAction;
class QContextMenuEvent;
class QMouseEvent;
class QLabel;

#include "user-interface_global.h"

namespace UserInterface
{
    class USERINTERFACESHARED_EXPORT ResizeableGridLayoutContainerButton : public QFrame
    {
        Q_OBJECT
    public:
        explicit ResizeableGridLayoutContainerButton(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

        int itemId() const;
        void setItemId(int id);

        QString text() const;
        void setText(const QString& labelText);

        int dragActivationThreshold() const;
        void setDragActivationThreshold(int threshold);

    signals:
        void selectInvoked(int itemId);
        void maximiseInvoked(int itemId);
        void closeInvoked(int itemId);
        void floatInvoked(int itemId, bool dragged);

    public slots:
        void currentItemIndexChanged(int index);

    protected:
        void contextMenuEvent(QContextMenuEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    private:
        void initLayout();
        void initActions();
        void updateTextElide();

        QAction* m_pSelectAction;
        QAction* m_pMaximiseAction;
        QAction* m_pFloatAction;
        QAction* m_pCloseAction;

        int m_iItemID;
        QLabel* m_pLabel;
        QString m_strLabelText;
        int m_iDragActivationThreshold;
        bool m_bInDrag;
        QPoint m_LastMousePress;
    };
}

#endif // RESIZEABLEGRIDLAYOUTCONTAINERBUTTON_H
