#include "rendermodelpasskey.h"

uint qHash(const NS_RENDERER::RenderModelPassKey &key, uint seed)
{
    return qHash(key.passIndex());
}

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
