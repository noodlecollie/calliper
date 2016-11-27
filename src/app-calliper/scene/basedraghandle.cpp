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
    // This is just about acceptable for our billboarding purposes.
    // It faces the object backwards along the view vector of the camera.
    setAngles(Math::vectorToAngleSimple((stack->worldToCameraTop().inverted() * QVector4D(0,-1,0,0)).toVector3D()));

    UIManipulator::draw(stack);
}
