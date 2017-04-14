#ifndef TEMPRENDER_H
#define TEMPRENDER_H

#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>

class TempRender
{
public:
    TempRender();
    ~TempRender();

    QOpenGLFramebufferObject* m_pFrameBuffer;
};

extern TempRender* tempRenderInstance;

#endif // TEMPRENDER_H
