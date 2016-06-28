#ifndef BASEDRAGHANDLE_H
#define BASEDRAGHANDLE_H

#include "uimanipulator.h"

class BaseDragHandle : public UIManipulator
{
    friend class BaseScene;
    Q_OBJECT
public:
    virtual void draw(ShaderStack *stack);

    void setDebugVar(bool b);

protected:
    explicit BaseDragHandle(BaseScene* scene, SceneObject *parent, const QString &iconTexture);
    BaseDragHandle(const BaseDragHandle &cloneFrom);
    virtual ~BaseDragHandle();

private:
    void buildSprite();

    QString     m_szIconTexture;
    bool        m_bDebugVar;
};

#endif // BASEDRAGHANDLE_H
