#include "resizeablegridlayoutcontainerhandle.h"
#include <QApplication>
#include <QDebug>

namespace UserInterface
{
    ResizeableGridLayoutContainerHandle::ResizeableGridLayoutContainerHandle(QWidget *parent)
        : QToolBar(parent)
    {
        init();
        createActions();
    }

    QSize ResizeableGridLayoutContainerHandle::sizeHint() const
    {
        QSize size;

        // There's probably a better way to do this.
        size.setWidth(10);

        QList<QAction*> actionList = actions();
        foreach ( QAction* action, actionList )
        {
            QWidget* w = widgetForAction(action);
            if ( !w )
                continue;

            QSize s = w->size();
            size.setHeight(s.height());
            size.setWidth(size.width() + s.width());
        }

        return size;
    }

    void ResizeableGridLayoutContainerHandle::init()
    {
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        setFloatable(false);
        setMovable(false);
        setIconSize(QSize(9,9));
    }

    void ResizeableGridLayoutContainerHandle::createActions()
    {
        createAction(m_pCloseAction, SIGNAL(closeClicked()), QStyle::SP_DockWidgetCloseButton);
        createAction(m_pMaximizeAction, SIGNAL(maximizeClicked()), QStyle::SP_TitleBarMaxButton);
        createAction(m_pFloatAction, SIGNAL(floatClicked()), QStyle::SP_TitleBarNormalButton);
    }

    void ResizeableGridLayoutContainerHandle::createAction(QAction *&member, const char *signal, QStyle::StandardPixmap pixmap)
    {
        QAction* action = new QAction;
        action->setIcon(QApplication::style()->standardIcon(pixmap));
        connect(action, SIGNAL(triggered(bool)), this, signal);
        addAction(action);
        member = action;
    }

    void ResizeableGridLayoutContainerHandle::mouseDoubleClickEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
        emit handleDoubleClicked();
    }
}
