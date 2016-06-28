#include "basedraghandle.h"
#include "basescene.h"
#include "shaderstack.h"
#include "shaders.h"
#include "geometryfactory.h"
#include <QtMath>

BaseDragHandle::BaseDragHandle(BaseScene *scene, SceneObject *parent, const QString &iconTexture) : UIManipulator(scene, parent)
{
    m_szIconTexture = iconTexture;
    m_bBillboard = false;
    m_bDebugVar = false;
    buildSprite();
}

BaseDragHandle::BaseDragHandle(const BaseDragHandle &cloneFrom) : UIManipulator(cloneFrom)
{
    m_szIconTexture = cloneFrom.m_szIconTexture;
    m_bBillboard = false;
    buildSprite();
}

BaseDragHandle::~BaseDragHandle()
{

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

void BaseDragHandle::draw(ShaderStack *stack)
{
    if ( m_bDebugVar )
    {
        lookAtGlobal((stack->worldToCameraTop().inverted() * QVector4D(0,0,0,1)).toVector3D());
    }

    UIManipulator::draw(stack);
}

void BaseDragHandle::setDebugVar(bool b)
{
    m_bDebugVar = b;
}
