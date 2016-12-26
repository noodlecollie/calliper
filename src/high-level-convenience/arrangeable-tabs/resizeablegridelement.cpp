#include "resizeablegridelement.h"
#include <QGridLayout>
#include <QPushButton>
#include "resizeablegridelementbutton.h"

namespace
{
    enum ResizeModeFlag
    {
        NoResizeFlag = 0,
        HorizontalResizeFlag = (1 << 0),
        VerticalResizeFlag = (1 << 1),
    };
}

namespace HighLevelConvenience
{
    ResizeableGridElement::ResizeableGridElement(QWidget *parent)
        : QWidget(parent),
          m_pGridLayout(nullptr),
          m_pBtnResizeRight(nullptr),
          m_pBtnResizeDown(nullptr),
          m_pBtnResizeDiag(nullptr)
    {
        m_pGridLayout = new QGridLayout();
        m_pGridLayout->setContentsMargins(0,0,0,0);
        m_pGridLayout->setSpacing(0);
        setLayout(m_pGridLayout);

        m_pBtnResizeRight = new ResizeableGridElementButton(ResizeableGridElementButton::HorizontalResizeFlag);
        connect(m_pBtnResizeRight, SIGNAL(mouseMoved(int,int)), this, SIGNAL(desiredSizeChanged(int,int)));
        m_pGridLayout->addWidget(m_pBtnResizeRight, 0, 1);

        m_pBtnResizeDown = new ResizeableGridElementButton(ResizeableGridElementButton::VerticalResizeFlag);
        connect(m_pBtnResizeDown, SIGNAL(mouseMoved(int,int)), this, SIGNAL(desiredSizeChanged(int,int)));
        m_pGridLayout->addWidget(m_pBtnResizeDown, 1, 0);

        m_pBtnResizeDiag = new ResizeableGridElementButton(ResizeableGridElementButton::HorizontalResizeFlag |
                                                           ResizeableGridElementButton::VerticalResizeFlag);
        connect(m_pBtnResizeDiag, SIGNAL(mouseMoved(int,int)), this, SIGNAL(desiredSizeChanged(int,int)));
        m_pGridLayout->addWidget(m_pBtnResizeDiag, 1, 1);
    }

    QWidget* ResizeableGridElement::contents() const
    {
        QLayoutItem* item = m_pGridLayout->itemAtPosition(0, 0);
        return item ? item->widget() : nullptr;
    }

    void ResizeableGridElement::setContents(QWidget *widget)
    {
        QWidget* w = contents();
        if ( w )
        {
            m_pGridLayout->removeWidget(w);
        }

        m_pGridLayout->addWidget(widget, 0, 0);
    }
}
