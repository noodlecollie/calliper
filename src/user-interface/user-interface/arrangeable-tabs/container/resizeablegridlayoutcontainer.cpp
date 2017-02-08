#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>

#include "resizeablegridlayoutcontainer.h"
#include "resizeablegridlayoutcontainerbutton.h"

namespace UserInterface
{
    ResizeableGridLayoutContainer::ResizeableGridLayoutContainer(QWidget *parent, Qt::WindowFlags f)
        : QFrame(parent, f),
          m_pGeneralLayout(nullptr),
          m_pButtonLayout(nullptr),
          m_pStackedWidget(nullptr)
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
            return nullptr;

        m_pButtonLayout->insertWidget(index, createButton(widget, index));
        m_pStackedWidget->insertWidget(index, widget);

        ResizeableGridLayoutContainerButton* oldButton = button(index+1);
        Q_ASSERT(oldButton);
        m_pButtonLayout->removeWidget(oldButton);
        delete oldButton;

        QWidget* oldWidget = m_pStackedWidget->widget(index+1);
        Q_ASSERT(oldWidget);
        m_pStackedWidget->removeWidget(oldWidget);
        oldWidget->setParent(nullptr);
        return oldWidget;
    }

    QWidget* ResizeableGridLayoutContainer::removeWidget(int index)
    {
        if ( index < 0 || index >= m_pStackedWidget->count() )
            return nullptr;

        ResizeableGridLayoutContainerButton* oldButton = button(index);
        Q_ASSERT(oldButton);
        m_pButtonLayout->removeWidget(oldButton);
        delete oldButton;

        QWidget* oldWidget = m_pStackedWidget->widget(index);
        Q_ASSERT(oldWidget);
        m_pStackedWidget->removeWidget(oldWidget);
        oldWidget->setParent(nullptr);
        return oldWidget;
    }

    QWidget* ResizeableGridLayoutContainer::removeCurrentWidget()
    {
        return removeWidget(currentWidgetIndex());
    }

    QWidget* ResizeableGridLayoutContainer::widgetAt(int index) const
    {
        if ( index < 0 || index >= m_pStackedWidget->count() )
            return nullptr;

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

        connect(this, SIGNAL(currentChanged(int)), button, SLOT(currentItemIndexChanged()));
        connect(button, SIGNAL(selectInvoked(int)), this, SLOT(buttonPressed(int)));
        connect(button, SIGNAL(maximiseInvoked(int)), this, SIGNAL(maximizeInvoked(int)));

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
