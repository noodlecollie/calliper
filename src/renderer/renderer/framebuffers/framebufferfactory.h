#ifndef FRAMEBUFFERFACTORY_H
#define FRAMEBUFFERFACTORY_H

#include "renderer_global.h"

#include <QHash>

#include "renderer/framebuffers/openglframebuffer.h"
#include "renderer/opengl/scopedcurrentcontext.h"

namespace Renderer
{
    // TODO: Move this into the model.
    class RENDERERSHARED_EXPORT FrameBufferFactory
    {
    public:
        FrameBufferFactory();
        ~FrameBufferFactory();

        template<typename... Args>
        OpenGLFrameBuffer* createFrameBuffer(Args... args)
        {
            quint8 nextId = acquireNextId();
            if ( nextId < 1 )
            {
                return Q_NULLPTR;
            }

            ScopedCurrentContext currentContext;
            Q_UNUSED(currentContext);

            OpenGLFrameBuffer* frameBuffer = new OpenGLFrameBuffer(nextId, std::move(args)...);
            m_FrameBufferTable.insert(nextId, frameBuffer);
            return frameBuffer;
        }

        template<typename... Args>
        OpenGLFrameBuffer* recreateFrameBuffer(quint8 oldId, Args... args)
        {
            if ( oldId < 1 || !m_FrameBufferTable.contains(oldId) )
            {
                return Q_NULLPTR;
            }

            destroyFrameBuffer(oldId);

            ScopedCurrentContext currentContext;
            Q_UNUSED(currentContext);

            OpenGLFrameBuffer* frameBuffer = new OpenGLFrameBuffer(oldId, std::move(args)...);
            m_FrameBufferTable.insert(oldId, frameBuffer);
            return frameBuffer;
        }

        void destroyFrameBuffer(quint8 id);

    private:
        quint8 acquireNextId();

        quint8 m_nNextId;
        QHash<quint8, OpenGLFrameBuffer*> m_FrameBufferTable;
    };
}

#endif // FRAMEBUFFERFACTORY_H
