#ifndef COPENGLRENDERER_H
#define COPENGLRENDERER_H

#include <QObject>
#include <QList>
#include <QOpenGLShader>

class QOpenGLContext;
class QOpenGLShaderProgram;
class CSceneObject;
class QOpenGLFunctions_3_2_Core;
class CBasicCamera;

class COpenGLRenderer : public QObject
{
    Q_OBJECT
public:
    //////////////////////////////////////////////////////
    /// Static Stuff
    //////////////////////////////////////////////////////

    // Formats for vertex buffers.
    // Attributes are interleaved in the order specified.
    enum InterleavingFormat
    {
        FormatPosition = 0,
        FormatPositionUV,
    };

    // Attribute types.
    // The value within this enum corresponds to the expected attribute
    // number used in the shader.
    enum Attribute
    {
        Position     = 0,
        UV,         // 1
    };

    // Shader types.
    enum ShaderType
    {
        SolidColor = 0,
    };

    // These are all IN BYTES.
    // Offset from the beginning of the buffer for the first instance of the given attribute.
    static int attributeOffset(InterleavingFormat format, Attribute att);

    // How large a given attribute is.
    static int attributeSize(Attribute att);

    // How large an entire collection of attributes is.
    // This is the same as the stride required for the format.
    static int interleavingFormatSize(InterleavingFormat format);

    //////////////////////////////////////////////////////

    explicit COpenGLRenderer(QObject *parent = 0);
    ~COpenGLRenderer();

    QOpenGLContext* backgroundContext() const;

    void initialise();
    bool isValid() const;
    bool initialiseAttempted() const;

    // Assumes context is current.
    void render(QOpenGLFunctions_3_2_Core* f, const CSceneObject* root, const CBasicCamera* camera);

signals:

public slots:

private:
    bool createBackgroundContext();
    bool compileShaders();

    QOpenGLContext* m_pBackgroundContext;
    QList<QOpenGLShaderProgram*>  m_ShaderList;
    bool    m_bInitialiseAttempted;
};

#endif // COPENGLRENDERER_H
