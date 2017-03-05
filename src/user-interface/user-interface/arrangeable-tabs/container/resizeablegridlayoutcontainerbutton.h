#ifndef RESIZEABLEGRIDLAYOUTCONTAINERBUTTON_H
#define RESIZEABLEGRIDLAYOUTCONTAINERBUTTON_H

#include <QFrame>
#include <QPointer>

class QAction;
class QContextMenuEvent;
class QMouseEvent;
class QLabel;
class QMenu;

#include "user-interface_global.h"

namespace UserInterface
{
    class ResizeableGridLayoutContainer;

    class USERINTERFACESHARED_EXPORT ResizeableGridLayoutContainerButton : public QFrame
    {
        Q_OBJECT
    public:
        ResizeableGridLayoutContainerButton(ResizeableGridLayoutContainer* container, QWidget *parent = Q_NULLPTR);
        ~ResizeableGridLayoutContainerButton();

        int itemId() const;
        void setItemId(int id);

        QString text() const;
        void setText(const QString& labelText);

    signals:
        void selectInvoked(int itemId);
        void maximiseInvoked(int itemId);
        void closeInvoked(int itemId);

    public slots:
        void currentItemIndexChanged();

    protected:
        void contextMenuEvent(QContextMenuEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void menuSelectInvoked();
        void menuMaximiseInvoked();
        void menuCloseInvoked();

    private:
        void initLayout();
        void updateTextElide();
        bool shouldHighlight() const;
        void setHighlighted(bool highlighted);

        ResizeableGridLayoutContainer* m_pContainer;

        int m_iItemID;
        QLabel* m_pLabel;
        QString m_strLabelText;
        bool m_bHighlighted;
    };
}

#endif // RESIZEABLEGRIDLAYOUTCONTAINERBUTTON_H
