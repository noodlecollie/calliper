#ifndef RESIZEABLEGRIDLAYOUTCONTAINERHANDLE_H
#define RESIZEABLEGRIDLAYOUTCONTAINERHANDLE_H

#include "user-interface_global.h"
#include <QToolBar>
#include <QAction>
#include <QStyle>

namespace UserInterface
{
    // TODO: Replace me with updated, stacked container.
    class ResizeableGridLayoutContainerHandle : public QToolBar
    {
        Q_OBJECT
    public:
        ResizeableGridLayoutContainerHandle(QWidget* parent = nullptr);

        virtual QSize sizeHint() const override;

    signals:
        void handleDoubleClicked();
        void closeClicked();
        void maximizeClicked();
        void floatClicked();

    protected:
        virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    private:
        void init();
        void createActions();
        void createAction(QAction*& member, const char* signal, QStyle::StandardPixmap pixmap);

        QAction* m_pCloseAction;
        QAction* m_pMaximizeAction;
        QAction* m_pFloatAction;
    };
}

#endif // RESIZEABLEGRIDLAYOUTCONTAINERHANDLE_H
