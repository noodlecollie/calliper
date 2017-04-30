#include "openglerrors.h"

#include <QtDebug>
#include <QTextStream>
#include <QOpenGLFunctions_4_1_Core>

QString OpenGLErrors::errorString(GLenum errorCode)
{
    int enumIndex = staticMetaObject.indexOfEnumerator("OpenGLError");
    QMetaEnum metaEnum = staticMetaObject.enumerator(enumIndex);
    const char* keyString = metaEnum.valueToKey(errorCode);
    return keyString ? keyString : QString("Unknown error code %1").arg(errorCode);
}

QString OpenGLErrors::debugOpenGLCapabilities()
{
    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();
    QString str;
    QTextStream s(&str);

    static const int propsWidth = 41;
    static const int valuesWidth = 12;

#define PROPS_START qSetFieldWidth(propsWidth)
#define VALUES_START qSetFieldWidth(valuesWidth)
#define FIELDS_END qSetFieldWidth(0) << endl

    for ( int i = 0; ; i++ )
    {
        switch(i)
        {
            case 0:
            {
                s << right << PROPS_START << "Property" << VALUES_START << "Value" << FIELDS_END;
                continue;
            }

            case 1:
            {
                QString str;
                for ( int i = 0; i < propsWidth + valuesWidth; i++ )
                {
                    str += "-";
                }
                s << str << FIELDS_END;
                continue;
            }

            case 2:
            {
                int n;
                f->glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
                s << right << PROPS_START << "GL_MAX_VERTEX_ATTRIBS" << VALUES_START << n << FIELDS_END;
                continue;
            }

            case 3:
            {
                int n;
                f->glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &n);
                s << right << PROPS_START << "GL_MAX_VERTEX_UNIFORM_VECTORS" << VALUES_START << n << FIELDS_END;
                continue;
            }

            case 4:
            {
                int n;
                f->glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &n);
                s << right << PROPS_START << "GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS" << VALUES_START << n << FIELDS_END;
                continue;
            }

            case 5:
            {
                int n;
                f->glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &n);
                s << right << PROPS_START << "GL_MAX_UNIFORM_BUFFER_BINDINGS" << VALUES_START << n << FIELDS_END;
                continue;
            }

            case 6:
            {
                int n;
                f->glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &n);
                s << right << PROPS_START << "GL_MAX_VERTEX_UNIFORM_BLOCKS" << VALUES_START << n << FIELDS_END;
                continue;
            }

            case 7:
            {
                int n;
                f->glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT , &n);
                s << right << PROPS_START << "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT" << VALUES_START << n << FIELDS_END;
                continue;
            }

            default:
            {
                break;
            }
        }

        break;
    }

#undef PROPS_START
#undef VALUES_START
#undef FIELDS_END

    s.flush();
    return str;
}
