#ifndef SHADERSTACK_H
#define SHADERSTACK_H

#include <QStack>
#include <QMatrix4x4>
#include <QColor>
#include <QVector3D>
#include "shaderprogram.h"

class Camera;
class OpenGLRenderer;

class ShaderStack
{
    friend class OpenGLRenderer;
public:
    ShaderStack(ShaderProgram* initial, bool autoUpdate = true);
    ~ShaderStack();

    void applyAll();
    bool inInitialState() const;

    const Camera* camera() const;
    void setCamera(const Camera* camera);

    bool autoUpdate() const;
    void setAutoUpdate(bool enabled);

    void shaderPush(ShaderProgram* program);
    void shaderPop();
    ShaderProgram* shaderTop() const;
    int shaderCount() const;

    void modelToWorldPreMultiply(const QMatrix4x4 &mat);
    void modelToWorldPostMultiply(const QMatrix4x4 &mat);
    void modelToWorldSetToIdentity();
    void modelToWorldPush();
    void modelToWorldPop();
    const QMatrix4x4& modelToWorldTop() const;
    int modelToWorldCount() const;
    void modelToWorldApply();

    void worldToCameraPreMultiply(const QMatrix4x4 &mat);
    void worldToCameraPostMultiply(const QMatrix4x4 &mat);
    void worldToCameraSetToIdentity();
    void worldToCameraPush();
    void worldToCameraPop();
    const QMatrix4x4& worldToCameraTop() const;
    int worldToCameraCount() const;
    void worldToCameraApply();

    void coordinateTransformPreMultiply(const QMatrix4x4 &mat);
    void coordinateTransformPostMultiply(const QMatrix4x4 &mat);
    void coordinateTransformSetToIdentity();
    void coordinateTransformPush();
    void coordinateTransformPop();
    const QMatrix4x4& coordinateTransformTop() const;
    int coordinateTransformCount() const;
    void coordinateTransformApply();

    void cameraProjectionPreMultiply(const QMatrix4x4 &mat);
    void cameraProjectionPostMultiply(const QMatrix4x4 &mat);
    void cameraProjectionSetToIdentity();
    void cameraProjectionPush();
    void cameraProjectionPop();
    const QMatrix4x4& cameraProjectionTop() const;
    int cameraProjectionCount() const;
    void cameraProjectionApply();

    void fogColorSetTop(const QColor &col);
    void fogColorPush();
    void fogColorPop();
    const QColor& fogColorTop() const;
    int fogColorCount() const;
    void fogColorApply();

    void fogBeginSetTop(float val);
    void fogBeginPush();
    void fogBeginPop();
    const float& fogBeginTop() const;
    int fogBeginCount() const;
    void fogBeginApply();

    void fogEndSetTop(float val);
    void fogEndPush();
    void fogEndPop();
    const float& fogEndTop() const;
    int fogEndCount() const;
    void fogEndApply();

    void directionalLightSetTop(const QVector3D &val);
    void directionalLightPush();
    void directionalLightPop();
    const QVector3D& directionalLightTop() const;
    int directionalLightCount() const;
    void directionalLightApply();

    void globalColorSetTop(const QColor &col);
    void globalColorPush();
    void globalColorPop();
    const QColor& globalColorTop() const;
    int globalColorCount() const;
    void globalColorApply();

    bool lockShader() const;

private:
    void preMultiplyTop(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att, const QMatrix4x4 &mat);
    void postMultiplyTop(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att, const QMatrix4x4 &mat);
    void setToIdentity(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att);
    void pop(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att);

    void setTop(QStack<QColor> &stack, ShaderProgram::Attribute att, const QColor &col);
    void pop(QStack<QColor> &stack, ShaderProgram::Attribute att);

    void setTop(QStack<float> &stack, ShaderProgram::Attribute att, float value);
    void pop(QStack<float> &stack, ShaderProgram::Attribute att);

    void setTop(QStack<QVector3D> &stack, ShaderProgram::Attribute att, const QVector3D &value);
    void pop(QStack<QVector3D> &stack, ShaderProgram::Attribute att);

    bool    m_bAutoUpdate;
    bool    m_bLockShader;
    const Camera*   m_pCamera;

    QStack<ShaderProgram*>  m_Shaders;
    QStack<QMatrix4x4>      m_ModelToWorld;
    QStack<QMatrix4x4>      m_WorldToCamera;
    QStack<QMatrix4x4>      m_CoordinateTransform;
    QStack<QMatrix4x4>      m_CameraProjection;
    QStack<QColor>          m_FogColor;
    QStack<float>           m_FogBegin;
    QStack<float>           m_FogEnd;
    QStack<QVector3D>       m_DirectionalLight;
    QStack<QColor>          m_GlobalColor;
};

#endif // SHADERSTACK_H
