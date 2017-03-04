#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QLabel>
#include <QHBoxLayout>
#include <QtDebug>
#include <QFontMetrics>
#include <QApplication>

#include "resizeablegridlayoutcontainerbutton.h"
#include "resizeablegridlayoutcontainer.h"
#include "resizeablegridlayoutcontainermenu.h"

namespace UserInterface
{
    ResizeableGridLayoutContainerButton::ResizeableGridLayoutContainerButton(ResizeableGridLayoutContainer *container, QWidget *parent)
        : QFrame(parent),
          m_pContainer(container),
          m_iItemID(-1),
          m_pLabel(nullptr),
          m_strLabelText(),
          m_bHighlighted(false)
    {
        Q_ASSERT(m_pContainer);
        initLayout();
    }

    ResizeableGridLayoutContainerButton::~ResizeableGridLayoutContainerButton()
    {
    }

    int ResizeableGridLayoutContainerButton::itemId() const
    {
        return m_iItemID;
    }

    void ResizeableGridLayoutContainerButton::setItemId(int id)
    {
        m_iItemID = id;
    }

    QString ResizeableGridLayoutContainerButton::text() const
    {
        return m_strLabelText;
    }

    void ResizeableGridLayoutContainerButton::setText(const QString &labelText)
    {
        if ( m_strLabelText == labelText )
            return;

        m_strLabelText = labelText;
        updateTextElide();
    }

    bool ResizeableGridLayoutContainerButton::shouldHighlight() const
    {
        return m_pContainer->widgetCount() > 1
                && m_pContainer->currentWidgetIndex() >= 0
                && m_pContainer->currentWidgetIndex() == m_iItemID;
    }

    void ResizeableGridLayoutContainerButton::currentItemIndexChanged()
    {
        if ( shouldHighlight() && !m_bHighlighted )
        {
            setHighlighted(true);
        }
        else if ( !shouldHighlight() && m_bHighlighted )
        {
            setHighlighted(false);
        }
    }

    void ResizeableGridLayoutContainerButton::setHighlighted(bool highlighted)
    {
        QPalette p = QApplication::palette();

        if ( highlighted )
        {
            p.setBrush(QPalette::Window, p.highlight());
            p.setBrush(QPalette::WindowText, p.highlightedText());
        }
        else
        {
            p.setBrush(QPalette::Window, p.window());
            p.setBrush(QPalette::WindowText, p.windowText());
        }

        setPalette(p);
        m_bHighlighted = highlighted;
    }

    void ResizeableGridLayoutContainerButton::initLayout()
    {
        QHBoxLayout* l = new QHBoxLayout();
        l->setContentsMargins(0,0,0,0);
        l->setSpacing(0);
        setLayout(l);

        m_pLabel = new QLabel();
        m_pLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_pLabel->setMinimumWidth(1);
        l->addWidget(m_pLabel);

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        setFrameStyle(QFrame::Panel | QFrame::Raised);
        setAutoFillBackground(true);
    }

    void ResizeableGridLayoutContainerButton::updateTextElide()
    {
        QFontMetrics fm(m_pLabel->font());
        m_pLabel->setText(fm.elidedText(m_strLabelText, Qt::ElideMiddle, m_pLabel->width()));
    }

    void ResizeableGridLayoutContainerButton::contextMenuEvent(QContextMenuEvent *event)
    {
        ResizeableGridLayoutContainerMenu* menu = new ResizeableGridLayoutContainerMenu(this);

        connect(menu, &ResizeableGridLayoutContainerMenu::selectInvoked, this, &ResizeableGridLayoutContainerButton::menuSelectInvoked);
        connect(menu, &ResizeableGridLayoutContainerMenu::maximiseInvoked, this, &ResizeableGridLayoutContainerButton::menuMaximiseInvoked);
        connect(menu, &ResizeableGridLayoutContainerMenu::closeInvoked, this, &ResizeableGridLayoutContainerButton::menuCloseInvoked);

        menu->popup(event->globalPos());
    }

    void ResizeableGridLayoutContainerButton::mousePressEvent(QMouseEvent *event)
    {
        switch ( event->button() )
        {
            case Qt::LeftButton:
            {
                emit selectInvoked(m_iItemID);
                return;
            }

            case Qt::MiddleButton:
            {
                emit closeInvoked(m_iItemID);
                return;
            }

            default:
                break;
        }

        QFrame::mousePressEvent(event);
    }

    void ResizeableGridLayoutContainerButton::mouseDoubleClickEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
        emit maximiseInvoked(m_iItemID);
    }

    void ResizeableGridLayoutContainerButton::resizeEvent(QResizeEvent *event)
    {
        QFrame::resizeEvent(event);
        updateTextElide();
    }

    void ResizeableGridLayoutContainerButton::menuCloseInvoked()
    {
        emit closeInvoked(m_iItemID);
    }

    void ResizeableGridLayoutContainerButton::menuMaximiseInvoked()
    {
        emit maximiseInvoked(m_iItemID);
    }

    void ResizeableGridLayoutContainerButton::menuSelectInvoked()
    {
        emit selectInvoked(m_iItemID);
    }
}
