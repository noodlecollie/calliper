#include "resizeablegridlayoutcontainer.h"
#include <QVBoxLayout>
#include "resizeablegridlayoutcontainerhandle.h"

namespace
{
    const int MARGIN = 1;
}

namespace UserInterface
{
    ResizeableGridLayoutContainer::ResizeableGridLayoutContainer(QWidget *parent, Qt::WindowFlags f)
        : QFrame(parent, f),
          m_pItem(nullptr)
    {
        QVBoxLayout* l = new QVBoxLayout();
        l->setContentsMargins(MARGIN, MARGIN, MARGIN, MARGIN);
        l->setSpacing(1);
        setLayout(l);

        initFrame();

        m_pTopHandle = new ResizeableGridLayoutContainerHandle();

        connect(m_pTopHandle, SIGNAL(handleDoubleClicked()), this, SLOT(handleFloatClicked()));
        connect(m_pTopHandle, SIGNAL(closeClicked()), this, SIGNAL(closeClicked()));
        connect(m_pTopHandle, SIGNAL(maximizeClicked()), this, SIGNAL(maximizeClicked()));
        connect(m_pTopHandle, SIGNAL(floatClicked()), this, SLOT(handleFloatClicked()));

        l->addWidget(m_pTopHandle);
    }

    QWidget* ResizeableGridLayoutContainer::item() const
    {
        return m_pItem;
    }

    QWidget* ResizeableGridLayoutContainer::replaceItem(QWidget *newItem)
    {
        if ( m_pItem )
        {
            vBoxLayout()->removeWidget(m_pItem);
        }

        QWidget* oldItem = m_pItem;
        m_pItem = newItem;

        if ( m_pItem )
        {
            vBoxLayout()->addWidget(m_pItem);
        }

        if ( oldItem )
        {
            oldItem->setParent(nullptr);
        }

        return oldItem;
    }

    void ResizeableGridLayoutContainer::setItem(QWidget *newItem)
    {
        QWidget* oldItem = replaceItem(newItem);
        if ( oldItem )
            delete oldItem;
    }

    QVBoxLayout* ResizeableGridLayoutContainer::vBoxLayout() const
    {
        return qobject_cast<QVBoxLayout*>(layout());
    }

    void ResizeableGridLayoutContainer::initFrame()
    {
        setFrameStyle(QFrame::Panel | QFrame::Sunken);
        setLineWidth(MARGIN);
    }

    bool ResizeableGridLayoutContainer::handleVisible() const
    {
        return m_pTopHandle->isVisible();
    }

    void ResizeableGridLayoutContainer::setHandleVisible(bool visible)
    {
        return m_pTopHandle->setVisible(visible);
    }

    void ResizeableGridLayoutContainer::handleFloatClicked()
    {
        emit floatClicked(item());
    }
}
