#ifndef GEOMETRYDATAKEY_H
#define GEOMETRYDATAKEY_H

#include <QHash>
#include <QOpenGLFunctions>

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"
#include "rendersystem/interface-classes/geometry/geometrysection.h"

struct GeometryDataKey
{
    RenderSystem::GeometrySection::DrawMode drawMode;
    float lineWidth;
    RenderSystem::RenderModelDefs::ObjectId objectId;
    RenderSystem::RenderModelDefs::SectionId sectionId;

    GeometryDataKey(RenderSystem::GeometrySection::DrawMode mode,
                    float width,
                    RenderSystem::RenderModelDefs::ObjectId object,
                    RenderSystem::RenderModelDefs::SectionId section);

    explicit GeometryDataKey(const RenderSystem::GeometrySection& section);

    bool operator <(const GeometryDataKey& other) const;
    bool operator ==(const GeometryDataKey& other) const;
};

uint qHash(const GeometryDataKey& key, uint seed = 0);

#endif // GEOMETRYDATAKEY_H
