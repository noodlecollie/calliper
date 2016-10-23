#include "debugtriangle.h"
#include "math/math.h"
#include <QtMath>

namespace NS_MODEL
{
    DebugTriangle::DebugTriangle(const SceneObjectInitParams &initParams, SceneObject* parentObject)
        : SceneObject(initParams, parentObject)
    {
        commonInit();
    }

    DebugTriangle::DebugTriangle(const DebugTriangle *cloneFrom, const SceneObjectInitParams &initParams)
        : SceneObject(cloneFrom, initParams)
    {
        commonInit();
    }

    DebugTriangle::~DebugTriangle()
    {

    }

    void DebugTriangle::bakeGeometry(const ShaderPalette &shaderPalette, NS_RENDERER::GeometryBuilder &builder) const
    {
        using namespace NS_RENDERER;

        GeometrySection* section =
                builder.createNewSection();

        section->addPosition(QVector3D(-1,-1,0));
        section->addPosition(QVector3D(1,-1,0));
        section->addPosition(QVector3D(0,1,0));

        section->addNormal(QVector3D(0,0,1));
        section->addNormal(QVector3D(0,0,1));
        section->addNormal(QVector3D(0,0,1));

        section->addTextureCoordinate(QVector2D(0,0));
        section->addTextureCoordinate(QVector2D(1,0));
        section->addTextureCoordinate(QVector2D(0.5f,1));

        section->addColor(QColor::fromRgb(0xffffffff));
        section->addColor(QColor::fromRgb(0xffffffff));
        section->addColor(QColor::fromRgb(0xffffffff));

        section->addIndexTriangle(0,1,2);
    }

    void DebugTriangle::commonInit()
    {

    }
}
