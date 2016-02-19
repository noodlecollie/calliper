#include "sceneobject.h"
#include "callipermath.h"
#include <cmath>
#include "mapscene.h"
#include "shaderstack.h"
#include "resourcemanager.h"
#include <QOpenGLTexture>
#include "mapdocument.h"

SceneObject::SceneObject(SceneObject *parent) : HierarchicalObject(parent)
{
    m_pScene = parent ? parent->m_pScene : NULL;
    m_pGeometry.reset(new GeometryData());
    m_iRenderFlags = 0;
    m_bHidden = false;
}

SceneObject::SceneObject(const SceneObject &cloneFrom) : HierarchicalObject(cloneFrom.parentObject())
{
    m_pScene = cloneFrom.m_pScene;
    m_pGeometry.reset(new GeometryData(*cloneFrom.m_pGeometry.data()));
    m_iRenderFlags = cloneFrom.m_iRenderFlags;
    m_bHidden = cloneFrom.m_bHidden;

    setPosition(cloneFrom.position());
    setAngles(cloneFrom.angles());
    setScale(cloneFrom.scale());
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

int SceneObject::renderFlags() const
{
    return m_iRenderFlags;
}

void SceneObject::setRenderFlags(int flags)
{
    m_iRenderFlags = flags;
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

void SceneObject::serialiseToJson(QJsonObject &obj) const
{
    // Set the identifier for this object.
    obj.insert(ISerialisable::KEY_IDENTIFIER(), QJsonValue(SceneObject::serialiseIdentifier()));

    // Serialise the parent object first.
    QJsonObject jsonParent;
    HierarchicalObject::serialiseToJson(jsonParent);

    // Insert this as the superclass.
    obj.insert(ISerialisable::KEY_SUPERCLASS(), QJsonValue(jsonParent));

    // Serialise geometry.
    if ( shouldSerialiseGeometry() )
    {
        QJsonObject jsonGeom;
        m_pGeometry->serialiseToJson(jsonGeom);
        obj.insert("geometry", QJsonValue(jsonGeom));
    }

    // Other properties
    obj.insert("renderFlags", QJsonValue(renderFlags()));
    obj.insert("hidden", QJsonValue(hidden()));
}

QString SceneObject::serialiseIdentifier() const
{
    return staticMetaObject.className();
}

bool SceneObject::shouldSerialiseGeometry() const
{
    // By default this is true.
    // Subclasses can override.
    return true;
}
