#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>

#include "resizeablegridlayoutcontainer.h"
#include "resizeablegridlayoutcontainerbutton.h"

namespace UserInterface
{
    ResizeableGridLayoutContainer::ResizeableGridLayoutContainer(QWidget *parent, Qt::WindowFlags f)
        : QFrame(parent, f),
          m_pGeneralLayout(Q_NULLPTR),
          m_pButtonLayout(Q_NULLPTR),
          m_pStackedWidget(Q_NULLPTR)
    {
        initLayout();
    }

    int ResizeableGridLayoutContainer::widgetCount() const
    {
        return m_pStackedWidget->count();
    }

    int ResizeableGridLayoutContainer::currentWidgetIndex() const
    {
        return m_pStackedWidget->currentIndex();
    }

    void ResizeableGridLayoutContainer::setCurrentWidgetIndex(int index)
    {
        m_pStackedWidget->setCurrentIndex(index);
    }

    void ResizeableGridLayoutContainer::addWidget(QWidget *widget)
    {
        if ( !widget )
            return;

        int index = m_pStackedWidget->count();
        m_pButtonLayout->addWidget(createButton(widget, index));
        m_pStackedWidget->addWidget(widget);
        emit currentChanged(m_pStackedWidget->currentIndex());
    }

    QWidget* ResizeableGridLayoutContainer::replaceWidget(int index, QWidget *widget)
    {
        if ( !widget || index < 0 || index >= m_pStackedWidget->count() )
            return Q_NULLPTR;

        m_pButtonLayout->insertWidget(index, createButton(widget, index));
        m_pStackedWidget->insertWidget(index, widget);

        ResizeableGridLayoutContainerButton* oldButton = button(index+1);
        Q_ASSERT(oldButton);
        m_pButtonLayout->removeWidget(oldButton);
        delete oldButton;

        QWidget* oldWidget = m_pStackedWidget->widget(index+1);
        Q_ASSERT(oldWidget);
        m_pStackedWidget->removeWidget(oldWidget);
        oldWidget->setParent(Q_NULLPTR);
        return oldWidget;
    }

    QWidget* ResizeableGridLayoutContainer::removeWidget(int index)
    {
        if ( index < 0 || index >= m_pStackedWidget->count() )
            return Q_NULLPTR;

        ResizeableGridLayoutContainerButton* oldButton = button(index);
        Q_ASSERT(oldButton);
        m_pButtonLayout->removeWidget(oldButton);
        delete oldButton;

        QWidget* oldWidget = m_pStackedWidget->widget(index);
        Q_ASSERT(oldWidget);
        m_pStackedWidget->removeWidget(oldWidget);
        oldWidget->setParent(Q_NULLPTR);
        return oldWidget;
    }

    QWidget* ResizeableGridLayoutContainer::removeCurrentWidget()
    {
        return removeWidget(currentWidgetIndex());
    }

    QWidget* ResizeableGridLayoutContainer::widgetAt(int index) const
    {
        if ( index < 0 || index >= m_pStackedWidget->count() )
            return Q_NULLPTR;

        return m_pStackedWidget->widget(index);
    }

    ResizeableGridLayoutContainerButton* ResizeableGridLayoutContainer::button(int index) const
    {
        return qobject_cast<ResizeableGridLayoutContainerButton*>(m_pButtonLayout->itemAt(index)->widget());
    }

    ResizeableGridLayoutContainerButton* ResizeableGridLayoutContainer::createButton(QWidget *widget, int index)
    {
        ResizeableGridLayoutContainerButton* button = new ResizeableGridLayoutContainerButton(this);
        button->setItemId(index);
        button->setText(widget->windowTitle());

        connect(this, &ResizeableGridLayoutContainer::currentChanged, button, &ResizeableGridLayoutContainerButton::currentItemIndexChanged);

        connect(button, &ResizeableGridLayoutContainerButton::selectInvoked, this, &ResizeableGridLayoutContainer::buttonPressed);
        connect(button, &ResizeableGridLayoutContainerButton::maximiseInvoked, this, &ResizeableGridLayoutContainer::maximizeInvoked);
        connect(button, &ResizeableGridLayoutContainerButton::closeInvoked, this, &ResizeableGridLayoutContainer::closeInvoked);

        return button;
    }

    void ResizeableGridLayoutContainer::buttonPressed(int index)
    {
        if ( index < 0 || index >= m_pStackedWidget->count() )
            return;

        m_pStackedWidget->setCurrentIndex(index);
    }

    void ResizeableGridLayoutContainer::initLayout()
    {
        setFrameStyle(QFrame::Panel | QFrame::Sunken);

        m_pGeneralLayout = new QVBoxLayout();
        m_pGeneralLayout->setContentsMargins(0,0,0,0);
        m_pGeneralLayout->setSpacing(0);
        setLayout(m_pGeneralLayout);

        m_pButtonLayout = new QHBoxLayout();
        m_pButtonLayout->setContentsMargins(0,0,0,0);
        m_pButtonLayout->setSpacing(0);

        m_pGeneralLayout->addLayout(m_pButtonLayout, 0);

        m_pStackedWidget = new QStackedWidget();
        connect(m_pStackedWidget, SIGNAL(currentChanged(int)), this, SIGNAL(currentChanged(int)));
        m_pGeneralLayout->addWidget(m_pStackedWidget, 1);
    }
}
