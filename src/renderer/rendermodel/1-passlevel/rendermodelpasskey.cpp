#include "rendermodelpasskey.h"

uint qHash(const Renderer::RenderModelPassKey &key, uint seed)
{
    return qHash(key.passIndex(), seed);
}

namespace Renderer
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
