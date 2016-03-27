#include "scalehandle.h"

ScaleHandle::ScaleHandle(SceneObject *parent) : UIManipulator(parent)
{

}

ScaleHandle::~ScaleHandle()
{

}

SceneObject* ScaleHandle::clone() const
{
    Q_ASSERT(false);
    return NULL;
}
