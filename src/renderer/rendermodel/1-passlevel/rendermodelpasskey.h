#ifndef RENDERMODELPASSKEY_H
#define RENDERMODELPASSKEY_H

#include "renderer_global.h"

namespace NS_RENDERER
{
    class RenderModelPassKey
    {
    public:
        RenderModelPassKey(int index);

        int passIndex() const;

        inline bool operator <(const RenderModelPassKey &other) const
        {
            return m_iPassIndex < other.m_iPassIndex;
        }

        inline bool operator ==(const RenderModelPassKey &other) const
        {
            return m_iPassIndex == other.m_iPassIndex;
        }

        inline bool operator !=(const RenderModelPassKey &other) const
        {
            return !(*this == other);
        }

    private:
        const int m_iPassIndex;
    };
}

#endif // RENDERMODELPASSKEY_H
