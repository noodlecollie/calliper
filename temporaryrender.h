#ifndef TEMPORARYRENDER_H
#define TEMPORARYRENDER_H

#include <QOpenGLFunctions_4_1_Core>

class QKeyEvent;

void temporarySetup(QOpenGLContext* context, QOpenGLFunctions_4_1_Core* f);
void temporaryRender(QOpenGLContext* context, QOpenGLFunctions_4_1_Core* f);
bool temporaryKeyPress(QKeyEvent* e);
bool temporaryKeyRelease(QKeyEvent* e);

#endif // TEMPORARYRENDER_H

