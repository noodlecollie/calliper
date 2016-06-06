#ifndef BASEDRAGHANDLE_H
#define BASEDRAGHANDLE_H

#include "uimanipulator.h"

class BaseDragHandle : public UIManipulator
{
    friend class BaseScene;
    Q_OBJECT
public:
    virtual SceneObject* clone() const;

protected:
    explicit BaseDragHandle(BaseScene* scene, SceneObject *parent, const QString &iconTexture);
    BaseDragHandle(const BaseDragHandle &cloneFrom);
    virtual ~BaseDragHandle();

private:
    QString     m_szIconTexture;
};

#endif // BASEDRAGHANDLE_H
