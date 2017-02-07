#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QLabel>
#include <QHBoxLayout>
#include <QtDebug>
#include <QFontMetrics>
#include <QApplication>

#include "resizeablegridlayoutcontainerbutton.h"

namespace UserInterface
{
    ResizeableGridLayoutContainerButton::ResizeableGridLayoutContainerButton(QWidget *parent, Qt::WindowFlags f)
        : QFrame(parent, f),
          m_pSelectAction(nullptr),
          m_pMaximiseAction(nullptr),
          m_pFloatAction(nullptr),
          m_pCloseAction(nullptr),
          m_iItemID(-1),
          m_pLabel(nullptr),
          m_strLabelText(),
          m_iDragActivationThreshold(0),
          m_bInDrag(false),
          m_LastMousePress(),
          m_bHighlighted(false)
    {
        initLayout();
        initActions();
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

    int ResizeableGridLayoutContainerButton::dragActivationThreshold() const
    {
        return m_iDragActivationThreshold;
    }

    void ResizeableGridLayoutContainerButton::setDragActivationThreshold(int threshold)
    {
        m_iDragActivationThreshold = threshold;
    }

    bool ResizeableGridLayoutContainerButton::shouldHighlight(int index) const
    {
        return index >= 0 && index == m_iItemID;
    }

    void ResizeableGridLayoutContainerButton::currentItemIndexChanged(int index)
    {
        if ( shouldHighlight(index) && !m_bHighlighted )
        {
            setHighlighted(true);
        }
        else if ( !shouldHighlight(index) && m_bHighlighted )
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

    void ResizeableGridLayoutContainerButton::initActions()
    {
        m_pSelectAction = new QAction(tr("&Select"));
        connect(m_pSelectAction, &QAction::triggered, [this]{ emit selectInvoked(m_iItemID); });

        m_pMaximiseAction = new QAction(tr("&Maximise"));
        connect(m_pMaximiseAction, &QAction::triggered, [this]{ emit maximiseInvoked(m_iItemID); });

        m_pFloatAction = new QAction(tr("&Float"));
        connect(m_pFloatAction, &QAction::triggered, [this]{ emit floatInvoked(m_iItemID, m_bInDrag); });

        m_pCloseAction = new QAction(tr("&Close"));
        connect(m_pCloseAction, &QAction::triggered, [this]{ emit closeInvoked(m_iItemID); });
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
        QMenu menu(this);

        menu.addAction(m_pSelectAction);
        menu.addAction(m_pMaximiseAction);
        menu.addAction(m_pFloatAction);
        menu.addSeparator();
        menu.addAction(m_pCloseAction);

        menu.exec(event->globalPos());
    }

    void ResizeableGridLayoutContainerButton::mousePressEvent(QMouseEvent *event)
    {
        switch ( event->button() )
        {
            case Qt::LeftButton:
            {
                m_bInDrag = true;
                m_LastMousePress = event->globalPos();
                m_pSelectAction->trigger();
                return;
            }

            case Qt::MiddleButton:
            {
                m_pCloseAction->trigger();
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
        m_pMaximiseAction->trigger();
    }

    void ResizeableGridLayoutContainerButton::mouseMoveEvent(QMouseEvent *event)
    {
        if ( !m_bInDrag )
        {
            QFrame::mouseMoveEvent(event);
            return;
        }

        if ( (event->globalPos() - m_LastMousePress).manhattanLength() >= m_iDragActivationThreshold )
        {
            m_pFloatAction->trigger();
            m_bInDrag = false;
        }
    }

    void ResizeableGridLayoutContainerButton::mouseReleaseEvent(QMouseEvent *event)
    {
        QFrame::mouseReleaseEvent(event);

        if ( event->button() == Qt::LeftButton )
        {
            m_bInDrag = false;
            m_LastMousePress = QPoint();
        }
    }

    void ResizeableGridLayoutContainerButton::resizeEvent(QResizeEvent *event)
    {
        QFrame::resizeEvent(event);
        updateTextElide();
    }
}
