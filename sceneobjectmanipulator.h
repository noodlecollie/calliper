#ifndef SCENEOBJECTMANIPULATOR_H
#define SCENEOBJECTMANIPULATOR_H

#include <QVector3D>
#include "eulerangle.h"

class SceneObject;

class SceneObjectManipulator
{
public:
    SceneObjectManipulator(SceneObject* obj, bool updateOnDestroy = true);
    ~SceneObjectManipulator();

    SceneObject* sceneObject() const;

    void resetTranslation();
    void resetAngles();
    void resetScale();
    void resetAll();

    QVector3D translation() const;
    void setTranslation(const QVector3D &vec);

    EulerAngle angles() const;
    void setAngles(const EulerAngle &angles);

    QVector3D scale() const;
    void setScale(const QVector3D &scale);

    void apply();

private:
    void updateCachedProperties();
    void updateObjectSoft();

    SceneObject*    m_pSceneObject;
    bool            m_bUpdateOnDestroy;

    QVector3D       m_vecCachedPosition;
    EulerAngle      m_angCachedAngles;
    QVector3D       m_vecCachedScale;

    QVector3D       m_vecTranslation;
    EulerAngle      m_angAngles;
    QVector3D       m_vecScale;
};

#endif // SCENEOBJECTMANIPULATOR_H
