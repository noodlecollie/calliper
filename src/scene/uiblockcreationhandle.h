#ifndef UIWIREFRAMEBOX_H
#define UIWIREFRAMEBOX_H

#include "uimanipulator.h"

class UIBlockCreationHandle : public UIManipulator
{
    Q_OBJECT
    friend class BaseScene;
public:
    BoundingBox bounds() const;
    void setBounds(const BoundingBox &bounds);
    void setBounds(const QVector3D &min, const QVector3D &max);
    virtual void draw(ShaderStack *stack);

protected:
    explicit UIBlockCreationHandle(BaseScene* scene, SceneObject *parent);
    UIBlockCreationHandle(const UIBlockCreationHandle &cloneFrom);
    virtual ~UIBlockCreationHandle();

private:
    void initDefaults();
    void rebuildGeometry();
    void createScaleHandles();
    void updateScaleHandles();
    void hideAllScaleHandles();

    BoundingBox     m_Bounds;
    bool            m_bRebuildGeometry;
    UIManipulator*  m_pScaleHandles[6];
};

#endif // UIWIREFRAMEBOX_H
