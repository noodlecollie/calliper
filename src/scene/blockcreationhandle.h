#ifndef BLOCKCREATIONHANDLE_H
#define BLOCKCREATIONHANDLE_H

#include "uimanipulator.h"
#include "boundingbox.h"

class BlockCreationHandle : public UIManipulator
{
    Q_OBJECT
    friend class BaseScene;
public:
    virtual void draw(ShaderStack *stack);

    BoundingBox bounds() const;
    void setBounds(const BoundingBox &bounds);
    void setBounds(const QVector3D &min, const QVector3D &max);

    bool drawFaces() const;
    void setDrawFaces(bool enabled);

protected:
    explicit BlockCreationHandle(BaseScene* scene, SceneObject *parent);
    BlockCreationHandle(const BlockCreationHandle &cloneFrom);
    virtual ~BlockCreationHandle();

signals:

public slots:

private:
    void initDefaults();
    void rebuildGeometry();

    BoundingBox m_Bounds;
    bool        m_bRebuildGeometry;
    bool        m_bDrawFaces;
};

#endif // BLOCKCREATIONHANDLE_H
