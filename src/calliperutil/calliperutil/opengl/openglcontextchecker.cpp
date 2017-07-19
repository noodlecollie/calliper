#include "openglcontextchecker.h"

namespace CalliperUtil
{
    OpenGLContextChecker::OpenGLContextChecker()
        : m_nContextAddress(0)
    {

    }

    void OpenGLContextChecker::snapCreationContext()
    {
        Q_ASSERT_X(m_nContextAddress == 0, Q_FUNC_INFO, "Attempting to snap context when creation context is already set!");

        QOpenGLContext* const context = QOpenGLContext::currentContext();
        Q_ASSERT_X(context, Q_FUNC_INFO, "Expected to be called with a current context!");

        m_nContextAddress = reinterpret_cast<quint64>(context);
    }

    void OpenGLContextChecker::clearCreationContext()
    {
        m_nContextAddress = 0;
    }


    bool OpenGLContextChecker::verifyCurrentContext() const
    {
        if ( m_nContextAddress == 0 )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Creation context is null!");
            return false;
        }

        QOpenGLContext* const context = QOpenGLContext::currentContext();
        const quint64 contextAddress = reinterpret_cast<quint64>(context);

        Q_ASSERT_X(contextAddress == m_nContextAddress, Q_FUNC_INFO, "Current context did not match creation context!");

        return contextAddress == m_nContextAddress;
    }
}
