#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QColor>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "matrixstack.h"
#include "eulerangle.h"
#include "shaderstack.h"

class Scene;
class SceneObject;
class Camera;
class GeometryData;
class ShaderProgram;
class QOpenGLFunctions_4_1_Core;

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

    QColor fogColor() const;
    void setFogColor(const QColor &col);

    float fogBeginDistance() const;
    void setFogBeginDistance(float dist);

    float fogEndDistance() const;
    void setFogEndDistance(float dist);

    void begin();
    void end();

    void renderScene(Scene* scene, const Camera* camera);
    SceneObject* selectFromDepthBuffer(Scene* scene, const Camera* camera, const QPoint &oglPos);

    // The quad is assumed to span [(-1,-1) (1,1)] with (0,0) being the centre.
    void drawQuad(GeometryData* quad, const QSize &screen, const QRect &subrect, Qt::Alignment alignment = Qt::AlignCenter,
                  int offset = 0, int count = -1);

    GeometryData* createTextQuad(const QSize &texSize, const QString &text, const QColor &col, const QFont &font,
                                                  Qt::Alignment alignment);

private:
    void renderSceneRecursive(SceneObject* obj, ShaderStack* stack);
    void renderSceneForSelection(QOpenGLFunctions_4_1_Core* functions, SceneObject* obj, ShaderStack* stack,
                                 const QPoint &selPos, SceneObject** selected, float nearestDepth = 1.0f);

    QColor      m_colGlobalColour;
    int         m_iShader;
    QVector3D   m_vecDirectionalLight;
    QColor      m_colFogColour;
    float       m_flFogBegin;
    float       m_flFogEnd;

    ShaderStack*    m_pStack;
    bool            m_bPreparedForRendering;
};

OpenGLRenderer* renderer();

#endif // OPENGLRENDERER_H
