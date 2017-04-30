#ifndef OPENGLHELPERS_H
#define OPENGLHELPERS_H

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_1_Core>

#define GL_CURRENT_F QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>()

#endif // OPENGLHELPERS_H
