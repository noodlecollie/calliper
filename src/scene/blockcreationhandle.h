#ifndef BLOCKCREATIONHANDLE_H
#define BLOCKCREATIONHANDLE_H

#include "uimanipulator.h"
#include "boundingbox.h"

class BlockCreationHandle : public UIManipulator
{
    Q_OBJECT
public:
    explicit BlockCreationHandle(SceneObject *parent = 0);
    virtual ~BlockCreationHandle();
    virtual SceneObject* clone() const;
    virtual void draw(ShaderStack *stack);

    BoundingBox bounds() const;
    void setBounds(const BoundingBox &bounds);
    void setBounds(const QVector3D &min, const QVector3D &max);

protected:
    BlockCreationHandle(const BlockCreationHandle &cloneFrom);

signals:

public slots:

private:
    void initDefaults();
    void rebuildGeometry();

    BoundingBox m_Bounds;
    bool        m_bRebuildGeometry;
};

#endif // BLOCKCREATIONHANDLE_H
