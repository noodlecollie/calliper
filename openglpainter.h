#ifndef OPENGLPAINTER_H
#define OPENGLPAINTER_H

#include <QStack>
#include <QMatrix4x4>
#include "shaderprogram.h"

class OpenGLPainter
{
public:
    OpenGLPainter(ShaderProgram* initial, bool autoUpdate = true);
    void applyAll();

    bool autoUpdate() const;
    void setAutoUpdate(bool enabled);

    void shaderPush(ShaderProgram* program);
    void shaderPop();
    ShaderProgram* shaderTop() const;
    int shaderCount() const;

    void modelToWorldPreMultiply(const QMatrix4x4 &mat);
    void modelToWorldPostMultiply(const QMatrix4x4 &mat);
    void modelToWorldPush();
    void modelToWorldPop();
    const QMatrix4x4& modelToWorldTop() const;
    int modelToWorldCount() const;

    void worldToCameraPreMultiply(const QMatrix4x4 &mat);
    void worldToCameraPostMultiply(const QMatrix4x4 &mat);
    void worldToCameraPush();
    void worldToCameraPop();
    const QMatrix4x4& worldToCameraTop() const;
    int worldToCameraCount() const;

    void coordinateTransformPreMultiply(const QMatrix4x4 &mat);
    void coordinateTransformPostMultiply(const QMatrix4x4 &mat);
    void coordinateTransformPush();
    void coordinateTransformPop();
    const QMatrix4x4& coordinateTransformTop() const;
    int coordinateTransformCount() const;

    void cameraProjectionPreMultiply(const QMatrix4x4 &mat);
    void cameraProjectionPostMultiply(const QMatrix4x4 &mat);
    void cameraProjectionPush();
    void cameraProjectionPop();
    const QMatrix4x4& cameraProjectionTop() const;
    int cameraProjectionCount() const;

private:
    void preMultiplyTop(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att, const QMatrix4x4 &mat);
    void postMultiplyTop(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att, const QMatrix4x4 &mat);
    void pop(QStack<QMatrix4x4> &stack, ShaderProgram::Attribute att);

    bool    m_bAutoUpdate;

    QStack<ShaderProgram*>  m_Shaders;
    QStack<QMatrix4x4>      m_ModelToWorld;
    QStack<QMatrix4x4>      m_WorldToCamera;
    QStack<QMatrix4x4>      m_CoordinateTransform;
    QStack<QMatrix4x4>      m_CameraProjection;
};

#endif // OPENGLPAINTER_H
