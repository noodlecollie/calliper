#include "sceneobject.h"
#include "callipermath.h"
#include <cmath>
#include "scene.h"
#include "shaderstack.h"
#include "resourcemanager.h"
#include <QOpenGLTexture>
#include "mapdocument.h"

SceneObject::SceneObject(SceneObject *parent) : HierarchicalObject(parent)
{
    m_pScene = parent ? parent->m_pScene : NULL;
    m_pGeometry.reset(new GeometryData());
    m_iRenderFlags = 0;
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

Scene* SceneObject::scene() const
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
