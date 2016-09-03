#include "scalehandle.h"
#include "geometryfactory.h"
#include "geometrydata.h"
#include "shaders.h"
#include <QtMath>
#include "basescene.h"

#define SCALE 0.5f
#define HEAD_RADIUS (SCALE * (0.15f/2.0f))

namespace NS_MODEL
{
    void addScaleHead(float radius, float translation, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
    {
        GeometryData* geom = GeometryFactory::cubeSolidColor(radius, col, true);
        geom->transform(Math::matrixTranslate(QVector3D(translation, 0, 0)));
        geom->transform(transform);
        data.append(*geom);
        delete geom;
    }

    void addScalePanel(float scale1, float scale2, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
    {
        GeometryData geometry;
        geometry.setShaderOverride(PerVertexColorShader::staticName());

        // Panel is in the XY plane.
        geometry.appendVertex(QVector3D(0,0,0), QVector3D(), col);
        geometry.appendVertex(QVector3D(scale1,0,0), QVector3D(), col);
        geometry.appendVertex(QVector3D(scale1,scale2,0), QVector3D(), col);
        geometry.appendVertex(QVector3D(0,scale2,0), QVector3D(), col);

        // Create two faces facing opposite directions.
        geometry.appendIndexTriangle(0,1,2);
        geometry.appendIndexTriangle(0,2,3);
        geometry.appendIndexTriangle(0,3,2);
        geometry.appendIndexTriangle(0,2,1);

        if ( !transform.isIdentity() )
        {
            geometry.transform(transform);
        }

        data.append(geometry);
    }

    void addScaleShaft(float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
    {
        GeometryData geometry;
        geometry.setShaderOverride(PerVertexColorShader::staticName());

        // Arrow shaft (line)
        geometry.appendVertex(QVector3D(0,0,0), QVector3D(), col);
        geometry.appendVertex(QVector3D(scale, 0, 0), QVector3D(), col);
        geometry.appendIndex(0);
        geometry.appendIndex(1);

        if ( !transform.isIdentity() )
        {
            geometry.transform(transform);
        }

        data.append(geometry);
    }

    ScaleHandle::ScaleHandle(BaseScene *scene, SceneObject *parent) : UIManipulator(scene, parent)
    {
        build();
    }

    ScaleHandle::~ScaleHandle()
    {

    }

    ScaleHandle::ScaleHandle(const ScaleHandle &cloneFrom) : UIManipulator(cloneFrom)
    {
        build();
    }

    void ScaleHandle::build(const QVector3D &scale)
    {
        clearGeometry();

        // Calculate maximum extents of the handle.
        float lengths[3];
        for ( int i = 0; i < 3; i++ )
        {
            lengths[i] = scale[i] * SCALE;
            if ( lengths[i] < 3 * HEAD_RADIUS )
                lengths[i] = 3 * HEAD_RADIUS;
        }

        GeometryData* heads = new GeometryData();
        GeometryData* lines = new GeometryData();

        heads->setDataFormat(GeometryData::PositionNormalColor);
        heads->setShaderOverride(PerVertexColorShader::staticName());
        heads->setDrawMode(GL_TRIANGLES);

        lines->setDataFormat(GeometryData::PositionNormalColor);
        lines->setShaderOverride(PerVertexColorShader::staticName());
        lines->setDrawMode(GL_LINES);

        addScaleHead(HEAD_RADIUS, lengths[0] - HEAD_RADIUS, QColor::fromRgb(PICKCOLOUR_X), QMatrix4x4(), *heads);
        addScaleHead(HEAD_RADIUS, lengths[1] - HEAD_RADIUS, QColor::fromRgb(PICKCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *heads);
        addScaleHead(HEAD_RADIUS, lengths[2] - HEAD_RADIUS, QColor::fromRgb(PICKCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *heads);

    //    addScalePanel(lengths[0]/4.0f, lengths[1]/4.0f, QColor::fromRgba(PICKCOLOUR_XY), QMatrix4x4(), *heads);
    //    addScalePanel(lengths[2]/4.0f, lengths[1]/4.0f, QColor::fromRgba(PICKCOLOUR_YZ), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *heads);
    //    addScalePanel(lengths[0]/4.0f, lengths[2]/4.0f, QColor::fromRgba(PICKCOLOUR_XZ), Math::matrixRotateX(qDegreesToRadians(90.0f)), *heads);

        addScaleShaft(lengths[0] - (2*HEAD_RADIUS), QColor::fromRgb(PICKCOLOUR_X), QMatrix4x4(), *lines);
        addScaleShaft(lengths[1] - (2*HEAD_RADIUS), QColor::fromRgb(PICKCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *lines);
        addScaleShaft(lengths[2] - (2*HEAD_RADIUS), QColor::fromRgb(PICKCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *lines);

        appendGeometry(heads);
        appendGeometry(lines);
    }

    void ScaleHandle::rescaleHandle(const QVector3D scale)
    {
        build(scale);
    }

    void ScaleHandle::draw(ShaderStack *stack)
    {
        UIManipulator::draw(stack);
    }
}
