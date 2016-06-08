#include "basedraghandle.h"
#include "basescene.h"
#include "shaderstack.h"
#include "shaders.h"
#include "geometryfactory.h"
#include <QtMath>

BaseDragHandle::BaseDragHandle(BaseScene *scene, SceneObject *parent, const QString &iconTexture) : UIManipulator(scene, parent)
{
    m_szIconTexture = iconTexture;
    buildSprite();
}

BaseDragHandle::BaseDragHandle(const BaseDragHandle &cloneFrom) : UIManipulator(cloneFrom)
{
    m_szIconTexture = cloneFrom.m_szIconTexture;
    buildSprite();
}

BaseDragHandle::~BaseDragHandle()
{

}

void BaseDragHandle::draw(ShaderStack *stack)
{
    QMatrix4x4 modMat = Math::matrixTranslate(QVector4D(stack->worldToCameraTop().column(3)).toVector3D()) * Math::StaticMatrix::ROT_Z_270();

    stack->worldToCameraPush();
    stack->worldToCameraSetToIdentity();
    stack->worldToCameraPostMultiply(modMat);

    UIManipulator::draw(stack);

    stack->worldToCameraPop();
}

void BaseDragHandle::buildSprite()
{
    if ( geometryCount() > 0 )
        clearGeometry();

    GeometryData* geom = GeometryFactory::triangleQuad(0.5f);
    geom->setShaderOverride(UnlitTextureShader::staticName());

    // This is built in XY plane and we want YZ plane, so we need to transform.
    geom->transform(QMatrix4x4(0,0,1,0,
                               1,0,0,0,
                               0,1,0,0,
                               0,0,0,1));

    // Set the texture.
    geom->setTexture(0, m_szIconTexture);
    appendGeometry(geom);
}
