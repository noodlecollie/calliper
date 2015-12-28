#include "originmarker.h"

OriginMarker::OriginMarker(SceneObject *parent) : SceneObject(parent)
{
    m_pGeometry->setDrawMode(GL_LINES);
    m_pGeometry->appendVertex(QVector3D(0,0,0), QColor(255,0,0));
    m_pGeometry->appendVertex(QVector3D(64,0,0), QColor(255,0,0));
    m_pGeometry->appendVertex(QVector3D(0,0,0), QColor(0,255,0));
    m_pGeometry->appendVertex(QVector3D(0,64,0), QColor(0,255,0));
    m_pGeometry->appendVertex(QVector3D(0,0,0), QColor(0,0,255));
    m_pGeometry->appendVertex(QVector3D(0,0,64), QColor(0,0,255));
    m_pGeometry->appendIndex(0);
    m_pGeometry->appendIndex(1);
    m_pGeometry->appendIndex(2);
    m_pGeometry->appendIndex(3);
    m_pGeometry->appendIndex(4);
    m_pGeometry->appendIndex(5);
}

