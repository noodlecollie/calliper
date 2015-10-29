#ifndef COPENGLRENDERER_H
#define COPENGLRENDERER_H

#include "cbaserenderer.h"

class QOpenGLContext;

class COpenGLRenderer : public CBaseRenderer
{
    Q_OBJECT
public:
    explicit COpenGLRenderer(QObject *parent = 0);

    QOpenGLContext* backgroundContext() const;

signals:

public slots:

private:
    bool createBackgroundContext();

    QOpenGLContext* m_pBackgroundContext;
};

#endif // COPENGLRENDERER_H
