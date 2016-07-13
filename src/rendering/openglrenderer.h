#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QColor>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "matrixstack.h"
#include "eulerangle.h"
#include "shaderstack.h"
#include <QMap>
#include "sceneobject.h"

class BaseScene;
class GeometryData;
class ShaderProgram;
class QOpenGLFunctions_4_1_Core;
class CameraLens;
class SceneCamera;
class QOpenGLTexture;

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

    void renderScene(BaseScene* scene, const CameraParams &params);
    SceneObject* selectFromDepthBuffer(BaseScene* scene, const CameraParams &params,
                                       const QPoint &oglPos, int selectionMask = SceneObject::AllObjectsMask, QRgb* pickColor = NULL);

    // The quad is assumed to span [(-1,-1) (1,1)] with (0,0) being the centre.
    void drawQuad(GeometryData* quad, const QSize &screen, const QRect &subrect, Qt::Alignment alignment = Qt::AlignCenter,
                  int offset = 0, int count = -1);

    GeometryData* createTextQuad(const QSize &texSize, const QString &text, const QColor &col, const QFont &font,
                                                  Qt::Alignment alignment);

    static QVector2D deviceCoordinates(const QVector3D &worldPos, const SceneCamera* camera);

private:
    class DeferredObject
    {
    public:
        DeferredObject(SceneObject* obj, const QMatrix4x4 &mat) :
            object(obj), matrix(mat)
        {
        }

        SceneObject* object;
        QMatrix4x4 matrix;
    };

    class ObjectPicker
    {
    public:
        ObjectPicker() :
            functions(NULL), selectionPos(), selectedObject(NULL), selectedColour(0xffffffff),
            nearestDepth(1.0f), getPickColour(false)
        {
        }

        ObjectPicker(QOpenGLFunctions_4_1_Core* f, const QPoint &p, bool getCol = false) :
            functions(f), selectionPos(p), selectedObject(NULL), selectedColour(0xffffffff),
            nearestDepth(1.0f), getPickColour(getCol)
        {
        }

        void checkDrawnObject(SceneObject* obj);

        QOpenGLFunctions_4_1_Core* functions;
        QPoint          selectionPos;
        SceneObject*    selectedObject;
        QRgb            selectedColour;
        float           nearestDepth;
        bool            getPickColour;
    };

    void renderSceneRecursive(SceneObject* obj, ShaderStack* stack);

    void clearDeferred();
    void renderDeferred();
    void renderTranslucent();
    void drawObject(SceneObject* obj, ShaderStack* stack);

    QColor      m_colGlobalColour;
    int         m_iShader;
    QVector3D   m_vecDirectionalLight;
    QColor      m_colFogColour;
    float       m_flFogBegin;
    float       m_flFogEnd;

    ShaderStack*    m_pStack;
    bool            m_bPreparedForRendering;

    QMap<float, DeferredObject> m_TranslucentObjects;
    QVector3D m_vecCurrentCameraWorldPosition;

    ObjectPicker m_ObjectPicker;
    bool m_bPicking;

    QOpenGLTexture* m_pRayTexture;
};

OpenGLRenderer* renderer();

#endif // OPENGLRENDERER_H
