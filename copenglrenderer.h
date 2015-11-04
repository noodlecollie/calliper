#ifndef COPENGLRENDERER_H
#define COPENGLRENDERER_H

#include "cbaserenderer.h"
#include <QList>

class QOpenGLContext;
class QOpenGLShaderProgram;

class COpenGLRenderer : public CBaseRenderer
{
    Q_OBJECT
public:
    explicit COpenGLRenderer(QObject *parent = 0);

    QOpenGLContext* backgroundContext() const;

    virtual void initialise();
    virtual bool isValid() const;

signals:

public slots:

private:
    bool createBackgroundContext();
    bool compileShaders();

    QOpenGLContext* m_pBackgroundContext;
    QList<QOpenGLShaderProgram*>  m_ShaderList;
};

#endif // COPENGLRENDERER_H
