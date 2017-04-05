#ifndef OPENGLHELPERS_H
#define OPENGLHELPERS_H

#include "renderer_global.h"
#include "renderer/global/mainrendercontext.h"

#define GL_MAIN_F QOpenGLFunctions_4_1_Core* f = Renderer::MainRenderContext::globalInstance()->versionFunctions()

#endif // OPENGLHELPERS_H
