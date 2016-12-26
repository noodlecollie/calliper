#ifndef RESIZEABLEGRIDLAYOUT_H
#define RESIZEABLEGRIDLAYOUT_H

#include "high-level-convenience_global.h"

class QGridLayout;

namespace HighLevelConvenience
{
    class ResizeableGridLayoutManager
    {
    public:
        explicit ResizeableGridLayoutManager(QGridLayout* gridLayout);

    private:
        QGridLayout* const m_pGridLayout;
    };
}

#endif // RESIZEABLEGRIDLAYOUT_H
