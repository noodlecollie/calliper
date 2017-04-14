#include "temprender.h"

TempRender* tempRenderInstance = Q_NULLPTR;

TempRender::TempRender()
    : m_pFrameBuffer(new QOpenGLFramebufferObject(QSize(128,128)))
{
}

TempRender::~TempRender()
{
    delete m_pFrameBuffer;
}
