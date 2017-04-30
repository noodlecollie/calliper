#include "openglshaderstore.h"

void OpenGLShaderStore::storeInitialised()
{
    // TODO
}

void OpenGLShaderStore::storeDestroyed()
{
    qDeleteAll(m_Objects.values());
}

