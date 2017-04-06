#include "framebufferfactory.h"

namespace Renderer
{
    FrameBufferFactory::FrameBufferFactory()
        : m_nNextId(1)
    {
    }

    FrameBufferFactory::~FrameBufferFactory()
    {
    }

    void FrameBufferFactory::destroyFrameBuffer(quint8 id)
    {
        if ( id < 1 || !m_FrameBufferTable.contains(id) )
        {
            return;
        }

        OpenGLFrameBuffer* frameBuffer = m_FrameBufferTable.take(id);
        if ( !frameBuffer )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Expected valid frame buffer!");
            return;
        }

        ScopedCurrentContext currentContext;
        Q_UNUSED(currentContext);

        if ( frameBuffer->isBound() )
        {
            frameBuffer->release();
        }

        delete frameBuffer;
    }

    quint8 FrameBufferFactory::acquireNextId()
    {
        Q_ASSERT_X(m_nNextId + 1 != 0, Q_FUNC_INFO, "Frame buffer factory ID overflow");
        return m_nNextId + 1 == 0 ? 0 : m_nNextId++;
    }
}
