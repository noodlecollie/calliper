#include "rendermodelpasskey.h"

namespace NS_RENDERER
{
    RenderModelPassKey::RenderModelPassKey(int index)
        : m_iPassIndex(index)
    {

    }

    int RenderModelPassKey::passIndex() const
    {
        return m_iPassIndex;
    }
}
