#include "originmarker.h"
#include "pervertexcolorshader.h"

OriginMarker::OriginMarker(SceneObject *parent) : SceneObject(parent)
{
    constructGeometry();
}

OriginMarker::~OriginMarker()
{
}

OriginMarker::OriginMarker(const OriginMarker &cloneFrom) : SceneObject(cloneFrom)
{
    m_pGeometry.reset(new GeometryData(*cloneFrom.m_pGeometry.data()));
}

bool OriginMarker::editable() const
{
    return false;
}

void OriginMarker::constructGeometry()
{
    m_pGeometry->setShaderOverride(PerVertexColorShader::staticName());

    m_pGeometry->setDrawMode(GL_LINES);
    m_pGeometry->appendVertex(QVector3D(0,0,0), QVector3D(), QColor(255,0,0));
    m_pGeometry->appendVertex(QVector3D(64,0,0), QVector3D(), QColor(255,0,0));
    m_pGeometry->appendVertex(QVector3D(0,0,0), QVector3D(), QColor(0,255,0));
    m_pGeometry->appendVertex(QVector3D(0,64,0), QVector3D(), QColor(0,255,0));
    m_pGeometry->appendVertex(QVector3D(0,0,0), QVector3D(), QColor(0,0,255));
    m_pGeometry->appendVertex(QVector3D(0,0,64), QVector3D(), QColor(0,0,255));
    m_pGeometry->appendIndex(0);
    m_pGeometry->appendIndex(1);
    m_pGeometry->appendIndex(2);
    m_pGeometry->appendIndex(3);
    m_pGeometry->appendIndex(4);
    m_pGeometry->appendIndex(5);
}

SceneObject* OriginMarker::clone() const
{
    return new OriginMarker(*this);
}
