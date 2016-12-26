#ifndef RESIZEABLEGRIDELEMENT_H
#define RESIZEABLEGRIDELEMENT_H

#include "high-level-convenience_global.h"
#include <QWidget>
#include <QHash>

class QPushButton;
class QGridLayout;

namespace HighLevelConvenience
{
    class ResizeableGridElementButton;

    class ResizeableGridElement : public QWidget
    {
        Q_OBJECT
    public:
        explicit ResizeableGridElement(QWidget* parent = 0);

        QWidget* contents() const;
        void setContents(QWidget* widget);

    signals:
        void desiredSizeChanged(int deltaX, int deltaY);

    private:
        QGridLayout* m_pGridLayout;
        ResizeableGridElementButton* m_pBtnResizeRight;
        ResizeableGridElementButton* m_pBtnResizeDown;
        ResizeableGridElementButton* m_pBtnResizeDiag;
    };
}

#endif // RESIZEABLEGRIDELEMENT_H
