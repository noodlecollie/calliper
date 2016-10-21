#include "originmarker.h"
#include "scene/scene.h"

namespace NS_MODEL
{
    OriginMarker::OriginMarker(const SceneObjectInitParams &initParams, SceneObject* parentObject)
        : SceneObject(initParams, parentObject)
    {
        commonInit();
    }

    OriginMarker::OriginMarker(const OriginMarker* cloneFrom, const SceneObjectInitParams &initParams)
        : SceneObject(cloneFrom, initParams)
    {
        commonInit();
    }

    OriginMarker::~OriginMarker()
    {

    }

    void OriginMarker::commonInit()
    {
        updateScalableState(scalable());
    }

    void OriginMarker::bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const
    {
        using namespace NS_RENDERER;

        GeometrySection* section = builder.createNewSection(
                    parentScene()->shaderStore()->shaderForCategory(ShaderStore::UnlitPerVertexColor),
                    0,
                    builder.modelToWorldMatrix());
        section->setDrawMode(GL_LINES);

        section->addPosition(QVector3D(0,0,0));
        section->addPosition(QVector3D(64,0,0));
        section->addPosition(QVector3D(0,0,0));
        section->addPosition(QVector3D(0,64,0));
        section->addPosition(QVector3D(0,0,0));
        section->addPosition(QVector3D(0,0,64));

        section->addColor(QColor::fromRgb(0xffff0000));
        section->addColor(QColor::fromRgb(0xffff0000));
        section->addColor(QColor::fromRgb(0xff00ff00));
        section->addColor(QColor::fromRgb(0xff00ff00));
        section->addColor(QColor::fromRgb(0xff0000ff));
        section->addColor(QColor::fromRgb(0xff0000ff));

        section->addIndex(0);
        section->addIndex(1);
        section->addIndex(2);
        section->addIndex(3);
        section->addIndex(4);
        section->addIndex(5);
    }

    bool OriginMarker::scalable() const
    {
        return false;
    }
}
