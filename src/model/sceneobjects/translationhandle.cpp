#include "translationhandle.h"
#include "geometryfactory.h"
#include "resourcemanager.h"
#include "shaderstack.h"
#include "shaderprogram.h"
#include "openglrenderer.h"
#include "shaders.h"
#include <cmath>
#include <QtMath>
#include "callipermath.h"
#include "basescene.h"

namespace NS_MODEL
{
    enum GeometrySections
    {
        XNormal = 0,
        YNormal,
        ZNormal,
        XYNormal,
        YZNormal,
        XZNormal,

        XHover,
        YHover,
        ZHover,
        XYHover,
        YZHover,
        XZHover,

        XShaftNormal = 0,
        YShaftNormal,
        ZShaftNormal,
        XShaftHover,
        YShaftHover,
        ZShaftHover
    };

    // When rendering for picking, we don't want to draw any hover states.
    const int PICKING_SECTIONS[] = { 0, 1, 2, 3, 4, 5 };

    void addTranslationHead(GeometrySections section, float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
    {
        static const float HEAD_RADIUS = 0.05f;
        static const float HEAD_LENGTH = 0.15f;

        GeometryData geometry;
        geometry.setShaderOverride(PerVertexColorShader::staticName());

        // Arrow head base circle
        for ( int i = 0; i < 8; i++ )
        {
            float radians = ((float)i * M_PI)/4.0f;
            QVector3D pos((1.0f - HEAD_LENGTH) * scale, HEAD_RADIUS * scale * qSin(radians), HEAD_RADIUS * scale * qCos(radians));
            geometry.appendVertex(pos, QVector3D(), col);
        }

        // Arrow head point
        int arrowPointIndex = geometry.vertexCount();
        geometry.appendVertex(QVector3D(scale,0,0), QVector3D(), col);

        // Arrow head circumference triangles
        for ( int i = 0; i < 8; i++ )
        {
            int j = i == 7 ? 0 : i+1;
            geometry.appendIndexTriangle(i, arrowPointIndex, j);
        }

        // Arrow head base triangles
        for ( int i = 1; i < 7; i++ )
        {
            geometry.appendIndexTriangle(0, i, i+1);
        }

        if ( !transform.isIdentity() )
        {
            geometry.transform(transform);
        }

        data.setSection((int)section, data.indexCount(), geometry.indexCount());
        data.setSectionCount(data.sectionCount()+1);
        data.append(geometry);
    }

    void addTranslationPanel(GeometrySections section, float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
    {
        GeometryData geometry;
        geometry.setShaderOverride(PerVertexColorShader::staticName());

        // Panel is in the XY plane.
        geometry.appendVertex(QVector3D(0,0,0), QVector3D(), col);
        geometry.appendVertex(QVector3D(scale,0,0), QVector3D(), col);
        geometry.appendVertex(QVector3D(scale,scale,0), QVector3D(), col);
        geometry.appendVertex(QVector3D(0,scale,0), QVector3D(), col);

        // Create two faces facing opposite directions.
        geometry.appendIndexTriangle(0,1,2);
        geometry.appendIndexTriangle(0,2,3);
        geometry.appendIndexTriangle(0,3,2);
        geometry.appendIndexTriangle(0,2,1);

        if ( !transform.isIdentity() )
        {
            geometry.transform(transform);
        }

        data.setSection((int)section, data.indexCount(), geometry.indexCount());
        data.setSectionCount(data.sectionCount()+1);
        data.append(geometry);
    }

    void addTranslationShaft(GeometrySections section, float scale, const QColor &col, const QMatrix4x4 &transform, GeometryData &data)
    {
        GeometryData geometry;
        geometry.setShaderOverride(PerVertexColorShader::staticName());

        // Arrow shaft (line)
        geometry.appendVertex(QVector3D(0,0,0), QVector3D(), col);
        geometry.appendVertex(QVector3D(0.85f * scale, 0, 0), QVector3D(), col);
        geometry.appendIndex(0);
        geometry.appendIndex(1);

        if ( !transform.isIdentity() )
        {
            geometry.transform(transform);
        }

        data.setSection((int)section, data.indexCount(), geometry.indexCount());
        data.setSectionCount(data.sectionCount()+1);
        data.append(geometry);
    }

    void TranslationHandle::build()
    {
        const float scale = 0.5f;
        GeometryData* arrows = new GeometryData();
        GeometryData* lines = new GeometryData();

        arrows->setDataFormat(GeometryData::PositionNormalColor);
        arrows->setShaderOverride(PerVertexColorShader::staticName());
        arrows->setDrawMode(GL_TRIANGLES);

        lines->setDataFormat(GeometryData::PositionNormalColor);
        lines->setShaderOverride(PerVertexColorShader::staticName());
        lines->setDrawMode(GL_LINES);

        // ============================================ //
        // Add normal geometry sections.
        addTranslationHead(XNormal, scale, QColor::fromRgb(PICKCOLOUR_X), QMatrix4x4(), *arrows);
        addTranslationHead(YNormal, scale, QColor::fromRgb(PICKCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *arrows);
        addTranslationHead(ZNormal, scale, QColor::fromRgb(PICKCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *arrows);

        // Add hover geometry sections.
        addTranslationHead(XHover, scale, QColor::fromRgb(HOVERCOLOUR_X), QMatrix4x4(), *arrows);
        addTranslationHead(YHover, scale, QColor::fromRgb(HOVERCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *arrows);
        addTranslationHead(ZHover, scale, QColor::fromRgb(HOVERCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *arrows);

        // Specify that these pieces should be drawn normal by default.
        m_ArrowSections.append((int)XNormal);
        m_ArrowSections.append((int)YNormal);
        m_ArrowSections.append((int)ZNormal);
        // ============================================ //

        // ============================================ //
        addTranslationPanel(XYNormal, scale/4.0f, QColor::fromRgba(PICKCOLOUR_XY), QMatrix4x4(), *arrows);
        addTranslationPanel(YZNormal, scale/4.0f, QColor::fromRgba(PICKCOLOUR_YZ), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *arrows);
        addTranslationPanel(XZNormal, scale/4.0f, QColor::fromRgba(PICKCOLOUR_XZ), Math::matrixRotateX(qDegreesToRadians(90.0f)), *arrows);

        addTranslationPanel(XYHover, scale/4.0f, QColor::fromRgba(HOVERCOLOUR_XY), QMatrix4x4(), *arrows);
        addTranslationPanel(YZHover, scale/4.0f, QColor::fromRgba(HOVERCOLOUR_YZ), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *arrows);
        addTranslationPanel(XZHover, scale/4.0f, QColor::fromRgba(HOVERCOLOUR_XZ), Math::matrixRotateX(qDegreesToRadians(90.0f)), *arrows);

        m_ArrowSections.append((int)XYNormal);
        m_ArrowSections.append((int)YZNormal);
        m_ArrowSections.append((int)XZNormal);
        // ============================================ //

        // ============================================ //
        addTranslationShaft(XShaftNormal, scale, QColor::fromRgb(PICKCOLOUR_X), QMatrix4x4(), *lines);
        addTranslationShaft(YShaftNormal, scale, QColor::fromRgb(PICKCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *lines);
        addTranslationShaft(ZShaftNormal, scale, QColor::fromRgb(PICKCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *lines);

        addTranslationShaft(XShaftHover, scale, QColor::fromRgb(HOVERCOLOUR_X), QMatrix4x4(), *lines);
        addTranslationShaft(YShaftHover, scale, QColor::fromRgb(HOVERCOLOUR_Y), Math::matrixRotateZ(qDegreesToRadians(90.0f)), *lines);
        addTranslationShaft(ZShaftHover, scale, QColor::fromRgb(HOVERCOLOUR_Z), Math::matrixRotateY(qDegreesToRadians(-90.0f)), *lines);

        m_ShaftSections.append((int)XShaftNormal);
        m_ShaftSections.append((int)YShaftNormal);
        m_ShaftSections.append((int)ZShaftNormal);
        // ============================================ //

        appendGeometry(arrows);
        appendGeometry(lines);
    }

    TranslationHandle::TranslationHandle(BaseScene *scene, SceneObject *parent) : UIManipulator(scene, parent)
    {
        build();
    }

    TranslationHandle::TranslationHandle(const TranslationHandle &cloneFrom) : UIManipulator(cloneFrom)
    {
        build();
    }

    TranslationHandle::~TranslationHandle()
    {

    }

    void TranslationHandle::setHoverAxis(int axis)
    {
        for ( int i = 0; i < m_ArrowSections.count(); i++ )
        {
            m_ArrowSections[i] = (axis == i) ? (int)(XHover + i) : (int)(XNormal + i);
        }

        for ( int i = 0; i < m_ShaftSections.count(); i++ )
        {
            m_ShaftSections[i] = (axis == i) ? (int)(XShaftHover + i) : (int)(XShaftNormal + i);
        }
    }

    void TranslationHandle::setHoverSection(QRgb pickColour)
    {
        setHoverAxis(axisIdentifierFromPickColor(pickColour));
    }

    void TranslationHandle::draw(ShaderStack *stack)
    {
        if ( isEmpty() )
            return;

        QVector4D testVec = stack->cameraProjectionTop() * stack->coordinateTransformTop() * stack->worldToCameraTop()
                * stack->modelToWorldTop() * QVector4D(0,0,0,1);

        stack->counterScalePush();
        stack->counterScaleSetTop(testVec.z());

        if ( stack->isPicking() )
        {
            drawGeometry(geometryAt(0).data(), stack, PICKING_SECTIONS, 6);
            drawGeometry(geometryAt(1).data(), stack, PICKING_SECTIONS, 3);
        }
        else
        {
            drawGeometry(geometryAt(0).data(), stack, m_ArrowSections.data(), m_ArrowSections.count());
            drawGeometry(geometryAt(1).data(), stack, m_ShaftSections.data(), m_ShaftSections.count());
        }

        stack->counterScalePop();
    }
}
