#ifndef GEOMETRYDATAKEY_H
#define GEOMETRYDATAKEY_H

#include <QHash>
#include <QOpenGLFunctions>

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

struct GeometryDataKey
{
    GLenum drawMode;
    float lineWidth;
    RenderSystem::RenderModelDefs::ObjectId objectId;
    RenderSystem::RenderModelDefs::SectionId sectionId;

    GeometryDataKey(GLenum mode,
                    float width,
                    RenderSystem::RenderModelDefs::ObjectId object,
                    RenderSystem::RenderModelDefs::SectionId section);

    bool operator <(const GeometryDataKey& other) const;
    bool operator ==(const GeometryDataKey& other) const;
};

uint qHash(const GeometryDataKey& key, uint seed = 0);

#endif // GEOMETRYDATAKEY_H
