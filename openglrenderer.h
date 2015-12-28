#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QColor>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "matrixstack.h"
#include "eulerangle.h"

class Scene;
class SceneObject;
class Camera;
class GeometryData;

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

    QVector3D directionalLight() const;
    void setDirectionalLight(const QVector3D &dir);
    void setDirectionalLight(const EulerAngle &ang);

    void renderScene(Scene* scene, const Camera* camera);

    GeometryData* createTextQuad(const QSize &texSize, const QString &text, const QColor &col, const QFont &font,
                                                  Qt::Alignment alignment);

private:
    void renderSceneRecursive(SceneObject* obj, MatrixStack &stack, const QMatrix4x4 &worldToCam, const QMatrix4x4 &projection);

    QColor      m_colGlobalColour;
    int         m_iShader;
    QVector3D   m_vecDirectionalLight;
};

OpenGLRenderer* renderer();

#endif // OPENGLRENDERER_H
