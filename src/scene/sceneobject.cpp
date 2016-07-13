#include "sceneobject.h"
#include "callipermath.h"
#include <cmath>
#include "mapscene.h"
#include "shaderstack.h"
#include "resourcemanager.h"
#include <QOpenGLTexture>
#include "mapdocument.h"
#include "jsonutil.h"
#include "basescene.h"
#include "geometryfactory.h"
#include "pervertexcolorshader.h"

SceneObject::SceneObject(BaseScene* scene, SceneObject *parent) : HierarchicalObject(parent)
{
    initDefaults(parent, scene);
}

SceneObject::SceneObject(const SceneObject &cloneFrom) : HierarchicalObject(cloneFrom.parentObject())
{
    Q_ASSERT(cloneFrom.m_pScene);
    Q_ASSERT(cloneFrom.parentObject());
    Q_ASSERT(&cloneFrom != cloneFrom.m_pScene->root());

    initDefaults();

    m_pScene = cloneFrom.m_pScene;
    m_RenderFlags = cloneFrom.m_RenderFlags;
    m_bHidden = cloneFrom.m_bHidden;
    m_bSerialiseGeometry = cloneFrom.m_bSerialiseGeometry;
    m_CachedBounds = cloneFrom.m_CachedBounds;
    m_bBoundsStale = cloneFrom.m_bBoundsStale;
    m_bUseCachedBounds = cloneFrom.m_bUseCachedBounds;

    deepCloneGeometryFrom(cloneFrom.m_GeometryList);

    setPosition(cloneFrom.position());
    setAngles(cloneFrom.angles());
    setScale(cloneFrom.scale());
}

void SceneObject::initDefaults()
{
    m_RenderFlags = NoRenderFlag;
    m_bHidden = false;
    m_bSerialiseGeometry = false;
    m_bBoundsStale = true;
    m_bUseCachedBounds = false;
    m_bDrawBounds = false;

    connect(this, &HierarchicalObject::orientationChanged, this, &SceneObject::onOrientationChanged);
}

void SceneObject::initDefaults(SceneObject* parent, BaseScene* scene)
{
    Q_ASSERT(scene);
    Q_ASSERT((parent && parent->m_pScene) || !scene->root());

    m_pScene = scene;
    initDefaults();
}

SceneObject::~SceneObject()
{
}

bool SceneObject::isEmpty() const
{
    return m_GeometryList.count() < 1;
}

SceneObject* SceneObject::parentObject() const
{
    return qobject_cast<SceneObject*>(parent());
}

QList<SceneObject*> SceneObject::children() const
{
    return findChildren<SceneObject*>(QString(), Qt::FindDirectChildrenOnly);
}

QList<SceneObject*> SceneObject::allChildren() const
{
    return findChildren<SceneObject*>(QString());
}

BaseScene* SceneObject::scene() const
{
    return m_pScene;
}

bool SceneObject::editable() const
{
    return true;
}

bool SceneObject::isBackground() const
{
    return false;
}

void SceneObject::draw(ShaderStack *stack)
{
    if ( isEmpty() )
        return;

    // If we're selected, set the global colour.
    bool pushedColor = false;
    MapDocument* doc = m_pScene->document();
    if ( doc->selectedSet().contains(this) )
    {
        pushedColor = true;
        stack->globalColorPush();
        stack->globalColorSetTop(doc->selectedColor());
    }

    for ( int i = 0; i < m_GeometryList.count(); i++ )
    {
        drawGeometry(m_GeometryList.at(i).data(), stack);
    }

    if ( pushedColor )
    {
        stack->globalColorPop();
    }
}

void SceneObject::drawGeometry(GeometryData *geom, ShaderStack *stack, const int *sections, int sectionCount)
{
    if ( geom->isEmpty() )
        return;

    bool shaderOverridden = false;

    // If we have a shader override, push the override.
    QString shaderOverrideName = geom->shaderOverride();
    if ( !shaderOverrideName.isNull() )
    {
        ShaderProgram* program = resourceManager()->shader(shaderOverrideName);
        if ( program )
        {
            shaderOverridden = true;
            stack->shaderPush(program);
        }
    }

    // Upload and bind the geometry.
    geom->upload();
    geom->bindVertices(true);
    geom->bindIndices(true);

    // Apply the data format.
    geom->applyDataFormat(stack->shaderTop());

    // Apply the texture.
    QOpenGLTexture* tex = resourceManager()->texture(geom->texture(0));
    tex->bind(0);

    // Draw.
    if ( !sections )
        geom->draw();
    else
        geom->drawSection(sections, sectionCount);

    // Pop the shader if we pushed one earlier.
    if ( shaderOverridden )
    {
        stack->shaderPop();
    }
}

SceneObject::RenderFlags SceneObject::renderFlags() const
{
    return m_RenderFlags;
}

void SceneObject::setRenderFlags(RenderFlags flags)
{
    m_RenderFlags = flags;
}

BoundingBox SceneObject::computeLocalBounds() const
{
    return totalGeometryBounds();
}

BoundingBox SceneObject::totalGeometryBounds() const
{
    BoundingBox bbox;
    for ( int i = 0; i < m_GeometryList.count(); i++ )
    {
        bbox.unionWith(m_GeometryList.at(i)->localBounds());
    }
    return bbox;
}

bool SceneObject::hidden() const
{
    return m_bHidden;
}

void SceneObject::setHidden(bool hide)
{
    m_bHidden = hide;
}

bool SceneObject::serialiseToJson(QJsonObject &obj) const
{
    // If we're not editable, we shouldn't be saved to a file.
    // This means that non-editable subclasses don't have to bother
    // with re-implementation of serialisation at all.
    if ( !editable() )
    {
        return false;
    }

    // Set the identifier for this object.
    obj.insert(ISerialisable::KEY_IDENTIFIER(), QJsonValue(SceneObject::serialiseIdentifier()));

    // Serialise the parent object first.
    QJsonObject jsonParent;
    HierarchicalObject::serialiseToJson(jsonParent);

    // Insert this as the superclass.
    obj.insert(ISerialisable::KEY_SUPERCLASS(), QJsonValue(jsonParent));

    // Serialise the name of this object.
    obj.insert("objectName", QJsonValue(objectName()));

    // Serialise geometry.
    obj.insert("serialiseGeometry", QJsonValue(shouldSerialiseGeometry()));
    if ( shouldSerialiseGeometry() )
    {
        serialiseAllGeometry(obj);
    }

    // Serialise render flags as an array.
    QJsonArray arrRenderFlags;
    JsonUtil::flagsToJsonArray<SceneObject, RenderFlags>(m_RenderFlags, "RenderFlags", arrRenderFlags);
    obj.insert("renderFlags", QJsonValue(arrRenderFlags));

    // Other properties
    obj.insert("hidden", QJsonValue(hidden()));
    obj.insert("drawBounds", QJsonValue(m_bDrawBounds));

    return true;
}

void SceneObject::serialiseAllGeometry(QJsonObject &obj) const
{
    if ( m_GeometryList.count() < 1 )
        return;

    QJsonArray arrGeometry;
    for ( int i = 0; i < m_GeometryList.count(); i++ )
    {
        QJsonObject g;
        m_GeometryList.at(i)->serialiseToJson(g);
        arrGeometry.append(QJsonValue(g));
    }

    obj.insert("geometry", QJsonValue(arrGeometry));
}

SceneObject::SceneObject(BaseScene* scene, const QJsonObject &serialisedData, SceneObject *parent) :
    HierarchicalObject(serialisedData.value(ISerialisable::KEY_SUPERCLASS()).toObject(), parent)
{
    initDefaults(parent, scene);

    // Make sure this object identifies us.
    if ( !validateIdentifier(serialisedData, SceneObject::serialiseIdentifier()) )
    {
        return;
    }

    QJsonValue vObjectName = serialisedData.value("objectName");
    if ( vObjectName.isString() )
    {
        setObjectName(vObjectName.toString());
    }

    QJsonValue vGeometry = serialisedData.value("geometry");
    if ( vGeometry.isArray() )
    {
        unserialiseAllGeometry(vGeometry.toArray());
    }

    QJsonValue vSerialiseGeometry = serialisedData.value("serialiseGeometry");
    if ( vSerialiseGeometry.isBool() )
    {
        setShouldSerialiseGeometry(vSerialiseGeometry.toBool());
    }

    QJsonValue vRenderFlags = serialisedData.value("renderFlags");
    if ( vRenderFlags.isArray() )
    {
        m_RenderFlags = JsonUtil::jsonArrayToFlags<SceneObject, RenderFlags>(vRenderFlags.toArray(), "RenderFlags");
    }

    QJsonValue vHidden = serialisedData.value("hidden");
    if ( vHidden.isBool() )
    {
        setHidden(vHidden.toBool());
    }

    QJsonValue vDrawBounds = serialisedData.value("drawBounds");
    if ( vDrawBounds.isBool() )
    {
        m_bDrawBounds = vDrawBounds.toBool();
    }
}

void SceneObject::unserialiseAllGeometry(const QJsonArray &geomArray)
{
    m_GeometryList.clear();
    for ( int i = 0; i < geomArray.count(); i++ )
    {
        m_GeometryList.append(GeometryDataPointer(new GeometryData(geomArray.at(i).toObject())));
    }
}

QString SceneObject::serialiseIdentifier() const
{
    return staticMetaObject.className();
}

bool SceneObject::shouldSerialiseGeometry() const
{
    return m_bSerialiseGeometry;
}

void SceneObject::setShouldSerialiseGeometry(bool enabled)
{
    m_bSerialiseGeometry = enabled;
}

void SceneObject::deepCloneGeometryFrom(const QVector<GeometryDataPointer> &list)
{
    m_GeometryList.clear();

    foreach ( GeometryDataPointer g, list )
    {
        m_GeometryList.append(GeometryDataPointer(new GeometryData(*(g.data()))));
    }
}

int SceneObject::geometryCount() const
{
    return m_GeometryList.count();
}

GeometryDataPointer SceneObject::geometryAt(int index) const
{
    return m_GeometryList.at(index);
}

void SceneObject::setGeometryAt(int index, const GeometryDataPointer &geom)
{
    Q_ASSERT(geom.data());
    m_GeometryList.replace(index, geom);
}

void SceneObject::appendGeometry(const GeometryDataPointer &geom)
{
    Q_ASSERT(geom.data());
    m_GeometryList.append(geom);
}

void SceneObject::removeGeometry(int index)
{
    m_GeometryList.remove(index);
}

void SceneObject::clearGeometry()
{
    m_GeometryList.clear();
}

bool SceneObject::computeIntersection(const Ray3D &ray, RayTraceContact &contact, RayCoordinateSpace space) const
{
    // By default we don't support this.
    Q_UNUSED(ray);
    Q_UNUSED(contact);
    Q_UNUSED(space);
    return false;
}

SceneObject* SceneObject::computeRayCastRecursive(const Ray3D &ray, RayTraceContact &contact)
{
    // TODO: Do initial testing with bounding boxes - would probably be quicker.

    Ray3D localRay = ray.transformed(parentToLocal());

    SceneObject* nearestObject = NULL;
    RayTraceContact nearestContact;
    bool hasNearestContact = false;

    // See if the ray hits us.
    // The intersection must be in front of the ray's origin point!
    RayTraceContact tempContact;
    bool intersected = computeIntersection(localRay, tempContact, IRayDetectable::LocalSpace);

    if ( intersected && Ray3D::inFrontOfOrigin(tempContact.rayParameter) )
    {
        nearestObject = this;
        nearestContact = tempContact;
        hasNearestContact = true;
    }

    // Now see if any of our children give intersections nearer than us.
    QList<SceneObject*> childList = children();
    SceneObject* tempObject = NULL;
    foreach ( SceneObject* child, childList )
    {
        RayTraceContact childContact;
        tempObject = child->computeRayCastRecursive(localRay, childContact);

        if ( tempObject && childContact.rayParameter < nearestContact.rayParameter && childContact.rayParameter > 0.0f )
        {
            nearestObject = tempObject;
            nearestContact = childContact;
            hasNearestContact = true;
        }
    }

    if ( hasNearestContact )
        contact = nearestContact;

    return nearestObject;
}

bool SceneObject::isRoot() const
{
    return parentObject() == NULL;
}

QList<SceneObject*> SceneObject::ancestors() const
{
    QList<SceneObject*> list;
    SceneObject* o = parentObject();
    while ( o )
    {
        list.prepend(o);
        o = o->parentObject();
    }

    return list;
}

bool SceneObject::passesObjectMask(int mask) const
{
    // If we are not editable but the non-editable mask is not set,
    // we should not be picked.
    if ( (mask & NotEditableMask) != NotEditableMask && !editable() )
        return false;

    return true;
}

void SceneObject::flagBoundsStale()
{
    m_bBoundsStale = true;

    SceneObject* p = parentObject();
    while ( p && !p->m_bBoundsStale )
    {
        p->m_bBoundsStale = true;
        p = p->parentObject();
    }
}

void SceneObject::unionOfChildBounds(BoundingBox &bbox) const
{
    QList<SceneObject*> childList = children();
    foreach ( SceneObject* c, childList )
    {
        bbox.unionWith(c->localToParent() * c->hierarchicalBounds());
    }
}

BoundingBox SceneObject::hierarchicalBounds(bool forceRecompute) const
{
    // Update if required.
    if ( m_bBoundsStale || !m_bUseCachedBounds || forceRecompute )
    {
        updateCachedBounds();
    }

    return m_CachedBounds;
}

void SceneObject::updateCachedBounds() const
{
    m_CachedBounds.setToNull();
    unionOfChildBounds(m_CachedBounds);
    m_CachedBounds.unionWith(computeLocalBounds());

    m_pBoundsGeom.reset(GeometryFactory::lineCuboid(m_CachedBounds, QColor::fromRgb(0xffff0000)));
    m_pBoundsGeom->setShaderOverride(PerVertexColorShader::staticName());

    m_bBoundsStale = false;
}

void SceneObject::onOrientationChanged()
{
    flagBoundsStale();
}

bool SceneObject::useCachedBounds() const
{
    return m_bUseCachedBounds;
}

void SceneObject::setUseCachedBounds(bool use)
{
    m_bUseCachedBounds = use;
}

bool SceneObject::drawBounds() const
{
    return m_bDrawBounds;
}

void SceneObject::setDrawBounds(bool draw)
{
    if ( draw == m_bDrawBounds )
        return;

    m_bDrawBounds = draw;

    if ( m_bDrawBounds )
        flagBoundsStale();
}

void SceneObject::drawBoundsGeometry(ShaderStack *stack)
{
    if ( m_bBoundsStale || !m_bUseCachedBounds )
    {
        updateCachedBounds();
    }

    drawGeometry(m_pBoundsGeom.data(), stack);
}
