#ifndef OPENGLHELPERS_H
#define OPENGLHELPERS_H

#include "renderer_global.h"
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLContext>

#define GL_CURRENT_F QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>()

#endif // OPENGLHELPERS_H
