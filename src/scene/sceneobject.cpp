#include "sceneobject.h"
#include "callipermath.h"
#include <cmath>
#include "mapscene.h"
#include "shaderstack.h"
#include "resourcemanager.h"
#include <QOpenGLTexture>
#include "mapdocument.h"
#include "jsonutil.h"

SceneObject::SceneObject(SceneObject *parent) : HierarchicalObject(parent)
{
    initDefaults(parent);
}

SceneObject::SceneObject(const SceneObject &cloneFrom) : HierarchicalObject(cloneFrom.parentObject())
{
    m_pScene = cloneFrom.m_pScene;
    m_RenderFlags = cloneFrom.m_RenderFlags;
    m_bHidden = cloneFrom.m_bHidden;
    m_bSerialiseGeometry = cloneFrom.m_bSerialiseGeometry;
    deepCloneGeometryFrom(cloneFrom.m_GeometryList);

    setPosition(cloneFrom.position());
    setAngles(cloneFrom.angles());
    setScale(cloneFrom.scale());
}

void SceneObject::initDefaults(SceneObject* parent)
{
    m_pScene = parent ? parent->m_pScene : NULL;
    m_RenderFlags = NoRenderFlag;
    m_bHidden = false;
    m_bSerialiseGeometry = false;
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

BaseScene* SceneObject::scene() const
{
    return m_pScene;
}

bool SceneObject::editable() const
{
    return true;
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

void SceneObject::drawGeometry(GeometryData *geom, ShaderStack *stack)
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
    geom->draw();

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

SceneObject* SceneObject::clone() const
{
    return new SceneObject(*this);
}

BoundingBox SceneObject::computeLocalBounds() const
{
    // Compute our bounds.
    BoundingBox bounds = totalGeometryBounds();

    // Union these with the bounds of all our children.
    QList<SceneObject*> childList = children();
    foreach ( SceneObject* o, childList )
    {
        bounds.unionWith(o->localToParent() * o->computeLocalBounds());
    }

    return bounds;
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

SceneObject::SceneObject(const QJsonObject &serialisedData, SceneObject *parent) : HierarchicalObject(serialisedData.value(ISerialisable::KEY_SUPERCLASS()).toObject(), parent)
{
    initDefaults(parent);

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

float SceneObject::computeIntersection(const Ray3D &ray, QRgb *col, RayCoordinateSpace space) const
{
    // By default we don't support this.
    Q_UNUSED(ray);
    Q_UNUSED(col);
    Q_UNUSED(space);
    return (float)qInf();
}

SceneObject* SceneObject::computeRayCastRecursive(const Ray3D &ray, float &intersection, QRgb *col)
{
    // TODO: Do initial testing with bounding boxes - would probably be quicker.

    Ray3D localRay = ray.transformed(parentToLocal());

    SceneObject* nearestObject = NULL;
    float nearestIntersection = (float)qInf();
    QRgb nearestColour = 0xff000000;

    // See if the ray hits us.
    // The intersection must be in front of the ray's origin point!
    QRgb tempColour = 0xff000000;
    float tempIntersection = computeIntersection(localRay, &tempColour, IRayDetectable::LocalSpace);

    if ( Ray3D::inFrontOfOrigin(tempIntersection) )
    {
        nearestIntersection = tempIntersection;
        nearestObject = this;
        nearestColour = tempColour;
    }

    // Now see if any of our children give intersections nearer than us.
    QList<SceneObject*> childList = children();
    SceneObject* tempObject = NULL;
    foreach ( SceneObject* child, childList )
    {
        tempObject = child->computeRayCastRecursive(localRay, tempIntersection, &tempColour);

        if ( tempObject && tempIntersection < nearestIntersection && tempIntersection > 0.0f )
        {
            nearestIntersection = tempIntersection;
            nearestObject = child;
            nearestColour = tempColour;
        }
    }

    intersection = nearestIntersection;
    if ( col )
        *col = nearestColour;

    return nearestObject;
}
