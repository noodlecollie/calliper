#ifndef TEMPORARYRENDER_H
#define TEMPORARYRENDER_H

#include <QOpenGLFunctions_4_1_Core>

void temporarySetup(QOpenGLContext* context, QOpenGLFunctions_4_1_Core* f);
void temporaryRender(QOpenGLContext* context, QOpenGLFunctions_4_1_Core* f);

#endif // TEMPORARYRENDER_H

