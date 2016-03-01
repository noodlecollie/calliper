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
    m_pGeometry.reset(new GeometryData(*cloneFrom.m_pGeometry.data()));
    m_RenderFlags = cloneFrom.m_RenderFlags;
    m_bHidden = cloneFrom.m_bHidden;
    m_bSerialiseGeometry = cloneFrom.m_bSerialiseGeometry;

    setPosition(cloneFrom.position());
    setAngles(cloneFrom.angles());
    setScale(cloneFrom.scale());
}

void SceneObject::initDefaults(SceneObject* parent)
{
    m_pScene = parent ? parent->m_pScene : NULL;
    m_pGeometry.reset(new GeometryData());
    m_RenderFlags = NoRenderFlag;
    m_bHidden = false;
    m_bSerialiseGeometry = false;
}

SceneObject::~SceneObject()
{
}

GeometryData* SceneObject::geometry() const
{
    return m_pGeometry.data();
}

void SceneObject::setGeometry(GeometryData *data)
{
    Q_ASSERT(data);
    if ( !data || m_pGeometry.data() == data ) return;

    m_pGeometry.reset(data);
}

bool SceneObject::isEmpty() const
{
    return m_pGeometry->isEmpty();
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
    bool shaderOverridden = false;
    if ( !geometry()->isEmpty() )
    {
        // If we have a shader override, push the override.
        QString shaderOverrideName = geometry()->shaderOverride();
        if ( !shaderOverrideName.isNull() )
        {
            ShaderProgram* program = resourceManager()->shader(shaderOverrideName);
            if ( program )
            {
                shaderOverridden = true;
                stack->shaderPush(program);
            }
        }
    }

    // Multiply the modelWorld matrix by our current one.
    // This updates the shader uniform too.
    // We do this even if the geometry is empty, so that the
    // transformation will apply recursively.
    // It is the caller's responsibility to manage pushing
    // and popping of the m2w matrix.
    stack->modelToWorldPostMultiply(localToParent());

    if ( !geometry()->isEmpty() )
    {
        // Upload and bind the geometry.
        geometry()->upload();
        geometry()->bindVertices(true);
        geometry()->bindIndices(true);

        // Apply the data format.
        geometry()->applyDataFormat(stack->shaderTop());

        // If we're selected, set the global colour.
        bool pushedColor = false;
        MapDocument* doc = m_pScene->document();
        if ( doc->selectedSet().contains(this) )
        {
            pushedColor = true;
            stack->globalColorPush();
            stack->globalColorSetTop(doc->selectedColor());
        }

        // Apply the texture.
        QOpenGLTexture* tex = resourceManager()->texture(geometry()->texture(0));
        tex->bind(0);

        // Draw.
        geometry()->draw();

        if ( pushedColor )
        {
            stack->globalColorPop();
        }

        // Pop the shader if we pushed one earlier.
        if ( shaderOverridden )
        {
            stack->shaderPop();
        }
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
    BoundingBox bounds = geometry()->localBounds();

    // Union these with the bounds of all our children.
    QList<SceneObject*> childList = children();
    foreach ( SceneObject* o, childList )
    {
        bounds.unionWith(o->localToParent() * o->computeLocalBounds());
    }

    return bounds;
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
        QJsonObject jsonGeom;
        m_pGeometry->serialiseToJson(jsonGeom);
        obj.insert("geometry", QJsonValue(jsonGeom));
    }

    // Serialise render flags as an array.
    QJsonArray arrRenderFlags;
    JsonUtil::flagsToJsonArray<SceneObject, RenderFlags>(m_RenderFlags, "RenderFlags", arrRenderFlags);
    obj.insert("renderFlags", QJsonValue(arrRenderFlags));

    // Other properties
    obj.insert("hidden", QJsonValue(hidden()));

    return true;
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
    if ( vGeometry.isObject() )
    {
        setGeometry(new GeometryData(vGeometry.toObject()));
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
