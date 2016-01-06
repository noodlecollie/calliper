#include "sceneobjectmanipulator.h"
#include "sceneobject.h"

SceneObjectManipulator::SceneObjectManipulator(SceneObject *obj)
{
    m_pSceneObject = obj;
    Q_ASSERT(m_pSceneObject);

    updateCachedProperties();
    resetAll();
}

SceneObjectManipulator::~SceneObjectManipulator()
{

}

void SceneObjectManipulator::updateCachedProperties()
{
    m_vecCachedPosition = m_pSceneObject->position();
    m_angCachedAngles = m_pSceneObject->angles();
    m_vecCachedScale = m_pSceneObject->scale();
}

SceneObject* SceneObjectManipulator::sceneObject() const
{
    return m_pSceneObject;
}

void SceneObjectManipulator::updateObjectSoft()
{
    m_pSceneObject->setPosition(m_vecCachedPosition + m_vecTranslation);
    m_pSceneObject->setScale(m_vecCachedScale * m_vecScale);
    m_pSceneObject->setAngles(m_angCachedAngles + m_angAngles);
}

void SceneObjectManipulator::resetTranslation()
{
    m_vecTranslation = QVector3D(0,0,0);
    updateObjectSoft();
}

void SceneObjectManipulator::resetScale()
{
    m_vecScale = QVector3D(1,1,1);
    updateObjectSoft();
}

void SceneObjectManipulator::resetAngles()
{
    m_angAngles = EulerAngle(0,0,0);
    updateObjectSoft();
}

void SceneObjectManipulator::resetAll()
{
    m_vecTranslation = QVector3D(0,0,0);
    m_vecScale = QVector3D(1,1,1);
    m_angAngles = EulerAngle(0,0,0);
    updateObjectSoft();
}

QVector3D SceneObjectManipulator::translation() const
{
    return m_vecTranslation;
}

void SceneObjectManipulator::setTranslation(const QVector3D &vec)
{
    m_vecTranslation = vec;
    updateObjectSoft();
}

EulerAngle SceneObjectManipulator::angles() const
{
    return m_angAngles;
}

void SceneObjectManipulator::setAngles(const EulerAngle &angles)
{
    m_angAngles = angles;
    updateObjectSoft();
}

QVector3D SceneObjectManipulator::scale() const
{
    return m_vecScale;
}

void SceneObjectManipulator::setScale(const QVector3D &scale)
{
    m_vecScale = scale;
    updateObjectSoft();
}

void SceneObjectManipulator::apply()
{
    updateObjectSoft();
    updateCachedProperties();
    resetAll();
}
