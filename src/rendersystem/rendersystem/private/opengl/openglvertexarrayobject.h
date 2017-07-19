#ifndef OPENGLVERTEXARRAYOBJECT_H
#define OPENGLVERTEXARRAYOBJECT_H

#include <QOpenGLFunctions>

#include "calliperutil/opengl/openglcontextchecker.h"

class VertexFormat;

/*
 * Some helpful documentation from good old StackOverflow:
 * https://stackoverflow.com/questions/26552642/when-is-what-bound-to-a-vao
 *
 * ```
 * The key thing to understand is that a VAO is a collection of state. It
 * does not own any data. It's VBOs that own vertex data. A VAO, on the
 * other hand, contains all the state used to describe where a draw call
 * gets its vertex attributes from. This includes, for each attribute:
 *
 * 1. If it's enabled.
 * 2. Which buffer the attribute is stored in.
 * 3. At which offset in the buffer the data starts.
 * 4. The spacing between subsequent attributes (aka the stride).
 * 5. The type of the data.
 * 6. The number of components.
 *
 * Plus, once only:
 *
 * 7. Which element array buffer is bound.
 *
 * Mapping this to API calls, the following calls change state tracked
 * by the currently bound VAO:
 *
 * glEnableVertexAttribArray(...)               (1)
 * glDisableVertexAttribArray(...)              (1)
 * glVertexAttribPointer(...)                   (2, 3, 4, 5, 6)
 * glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ...)   (7)
 *
 * Note that this does not include the current binding of GL_ARRAY_BUFFER.
 * The buffer used for each attribute is tracked indirectly, based on which
 * buffer was bound when glVertexAttribPointer() is called for the specific
 * attribute.
 * ```
 *
 * Addidionally:
 * https://gamedev.stackexchange.com/questions/99236/what-state-is-stored-in-an-opengl-vertex-array-object-vao-and-how-do-i-use-the
 *
 * ```
 * You may ask why it doesn't remember VBO binding. Because you don't need
 * to bind VBO to draw something, you only need to bind it when creating VAO:
 * When you call glVertexAttribPointer(...), VAO remembers what VBO is
 * currently bound. And VAO will take attributes from these VBOs when you
 * draw it, even if these VBOs are not bound currently.
 * ```
 *
 * Therefore, setup of a VAO should include creating VBOs, binding them and
 * specifying the vertex attributes.
 */

/*
 * VAOS. ARE. NOT. SHAREABLE.
 * If any functions are called on a VAO while the render system context is not current,
 * an assertion error will result. VAOs can ONLY be used from within the render system context!
 */

class OpenGLVertexArrayObject : private OpenGLContextChecker
{
public:
    OpenGLVertexArrayObject();
    ~OpenGLVertexArrayObject();

    bool create();
    void destroy();
    bool isCreated() const;

    bool bind();
    void release();

    // These functions assume the VAO is bound.
    void enableAttributeArrays(const VertexFormat& format);
    void disableAttributeArrays(const VertexFormat& format);
    void disableAttributeArrays();

private:
    void checkContext() const;

    GLuint  m_iVAOID;
};

#endif // OPENGLVERTEXARRAYOBJECT_H
