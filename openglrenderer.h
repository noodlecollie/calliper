#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QColor>
#include <QOpenGLFunctions>

class OpenGLRenderer
{
public:
    static void initialise();
    static void shutdown();

    OpenGLRenderer();
    ~OpenGLRenderer();
    void setUpOpenGLResources();

    QColor globalColor() const;
    void setGlobalColor(const QColor &col);

    int shaderIndex() const;
    void setShaderIndex(int index);

private:
    QColor  m_colGlobalColour;
    int     m_iShader;
};

OpenGLRenderer* renderer();

#endif // OPENGLRENDERER_H
