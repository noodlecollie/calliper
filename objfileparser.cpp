#include "objfileparser.h"

#define IDSHORT_POSITION    (('v' << 8) | 0)
#define IDSHORT_NORMAL      (('v' << 8) | 'n')
#define IDSHORT_UV          (('v' << 8) | 't')
#define IDSHORT_FACE        (('f' << 8) | 0)
#define IDSHORT_COMMENT     (('#' << 8) | 0)
#define MAX_IDENTIFIER_LENGTH   2

#define FACEATT_POSITION    0
#define FACEATT_UV          1
#define FACEATT_NORMAL      2

// No idea whether this is capable, but let's fuck with some bits.
uint qHash(const ObjFileParser::FaceVertex &key, uint seed = 0)
{
    return (uint)(key.atts[0] + (key.atts[1] * key.atts[1]) + (key.atts[2]  * key.atts[2] * key.atts[2])) ^ seed;
}

// Potentially faster? Assumes an array of length at least 2,
// and where any chars after the last valid char are 0.
ObjFileParser::IdentifierToken ObjFileParser::shortIdentifier(const char* id)
{
    unsigned short num = ((*id) << 8) | *(id+1);
    switch (num)
    {
        case IDSHORT_POSITION:
            return Position;

        case IDSHORT_NORMAL:
            return Normal;

        case IDSHORT_UV:
            return UV;

        case IDSHORT_FACE:
            return Face;

        case IDSHORT_COMMENT:
            return Comment;

        default:
            return Other;
    }
}

static bool isWhiteSpace(const char* c)
{
    switch (*c)
    {
        case ' ':
        case '\t':
            return true;

        default:
            return false;
    }
}

// Assumes begin points to a non-whitespace character.
ObjFileParser::IdentifierToken ObjFileParser::getNextIdentifierToken(const char* begin, const char* final)
{
    const int idLen = MAX_IDENTIFIER_LENGTH + 1;
    char identifier[idLen];
    memset(identifier, 0, idLen*sizeof(char));

    for (int i = 0; i < idLen-1 && (begin+i) <= final; i++)
    {
        if ( isWhiteSpace(begin+i) )
            break;

        identifier[i] = *(begin+i);
    }

    return shortIdentifier(identifier);
}

// We are given the final character so that we don't go over it.
// This must be >= begin!
static int nextNewline(const char* begin, const char* final)
{
    int advance = 0;
    while ( true )
    {
        if ( *begin == '\n' )
        {
            return advance;
        }

        if ( begin == final )
        {
            return -1;
        }

        advance++;
        begin++;
    }
}

template<typename T>
static int parseFloats(const QByteArray &line, QList<T> &list, int floatCount, ObjFileParser::ParseError &error)
{
    const char* begin = line.constData();
    int count = line.count();
    int startIndex = 0;

    // Wind past the first token.
    while ( startIndex < count && !isWhiteSpace(begin+startIndex) )
        startIndex++;

    // If we reached count, there was no whitespace.
    if ( startIndex >= count )
    {
        error = ObjFileParser::InvalidLineFormat;
        return 0;
    }

    int cur = startIndex;
    T listItem;
    for ( int i = 0; i < floatCount; i++ )
    {
        // Wind past any whitespace.
        while ( cur < count && isWhiteSpace(begin+cur) )
            cur++;

        // If we reached count, there was no other value to read.
        if ( cur >= count )
        {
            error = ObjFileParser::InvalidLineFormat;
            return 0;
        }

        int tokenStart = cur;

        // Wind past non-whitespace.
        while ( cur < count && !isWhiteSpace(begin+cur) )
            cur++;

        if ( cur >= count )
        {
            // If we reached count and this was not the last iteration,
            // there were not enough positional values on this line.
            if ( i < floatCount-1 )
            {
                error = ObjFileParser::InvalidLineFormat;
                return 0;
            }
        }

        // Parse the value.
        bool ok = false;
        float value = QString(QByteArray(begin+tokenStart, cur-tokenStart)).toFloat(&ok);
        if ( !ok )
        {
            error = ObjFileParser::InvalidNumber;
            return tokenStart;
        }

        listItem[i] = value;

        // Advance the current index.
        cur++;
    }

    list.append(listItem);

    error = ObjFileParser::NoError;
    return -1;
}

void ObjFileParser::parseFaceVertex(const QByteArray &token, ObjFileParser::FaceVertex &fv)
{
    QList<QByteArray> list = token.split('/');

    switch (list.count())
    {
        // Position only.
        case 1:
        {
            bool ok = false;
            fv.atts[FACEATT_POSITION] = QString(list.at(0)).toInt(&ok);
            if ( !ok )
            {
                memset(&fv, 0, sizeof(FaceVertex));
                return;
            }

            return;
        }

        // Position, plus either UV or normal.
        case 2:
        {
            // To find out whether the second item is UV or normal,
            // look for the first occurrence of a slash.
            int slash = token.indexOf('/');

            // If the next character is also a slash, the second item is a normal.
            // Otherwise it's a UV.
            int index = token.at(slash+1) != '/' ? FACEATT_UV : FACEATT_NORMAL;

            bool ok = false;
            fv.atts[FACEATT_POSITION] = QString(list.at(0)).toInt(&ok);
            if ( !ok )
            {
                memset(&fv, 0, sizeof(FaceVertex));
                return;
            }

            ok = false;
            fv.atts[index] = QString(list.at(1)).toInt(&ok);
            if ( !ok )
            {
                memset(&fv, 0, sizeof(FaceVertex));
                return;
            }

            return;
        }

        // All attributes.
        case 3:
        {
            for ( int i = 0; i < 3; i++ )
            {
                bool ok = false;
                fv.atts[i] = QString(list.at(i)).toInt(&ok);
                if ( !ok )
                {
                    memset(&fv, 0, sizeof(FaceVertex));
                    return;
                }
            }

            return;
        }

        default:
            memset(&fv, 0, sizeof(FaceVertex));
            return;
    }
}

int ObjFileParser::parseFace(const QByteArray &line, QList<ObjFileParser::FaceVertex> &fvList, ObjFileParser::ParseError &error)
{
    const char* begin = line.constData();
    int count = line.count();
    int cur = 0;

    // Wind past the first token.
    while ( cur < count && !isWhiteSpace(begin+cur) )
        cur++;

    while (cur < count)
    {
        // Wind past any whitespace.
        while ( cur < count && isWhiteSpace(begin+cur) )
            cur++;

        // If we reached count, break.
        if ( cur >= count )
        {
            break;
        }

        int tokenStart = cur;

        // Wind past non-whitespace.
        while ( cur < count && !isWhiteSpace(begin+cur) )
            cur++;

        // Get information for this vertex.
        FaceVertex fv;
        parseFaceVertex(QByteArray(begin+tokenStart, cur-tokenStart), fv);

        // If this vertex is not valid, exit out.
        if ( !fv.isValid() )
        {
            error = ObjFileParser::InvalidVertexRef;
            return tokenStart;
        }

        // Add this vertex to the list.
        fvList.append(fv);
    }

    // If we didn't read enough vertices, return an invalid face error.
    if ( fvList.count() < 3 )
    {
        error = ObjFileParser::InvalidFace;
        return 0;
    }

    // We reached the end of the line.
    error = ObjFileParser::NoError;
    return -1;
}

ObjFileParser::ObjFileParser()
{
}

ObjFileParser::ParseResult ObjFileParser::fillAttributes(const QByteArray &arr, QList<QVector3D> &positions, QList<QVector3D> &normals, QList<QVector2D> &uvs,
                    QList<unsigned int> &indices)
{
    m_pArray = &arr;
    m_IndexTable.clear();
    const char* base = arr.constData();
    m_pFinal = base + (arr.count()-1);
    memset(&m_Result, 0, sizeof(ObjFileParser::ParseResult));

    m_LocalPositions.clear();
    m_LocalNormals.clear();
    m_LocalUVs.clear();

    for ( m_iPos = 0; m_iPos < arr.count(); m_iPos++ )
    {
        m_pCur = base + m_iPos;

        // Find the first non-whitespace character.
        if ( isWhiteSpace(m_pCur) || *m_pCur == '\n' )
            continue;

        // Get an identifier for the token.
        IdentifierToken idt = getNextIdentifierToken(m_pCur, m_pFinal);

        // If the identifier is a comment, or something else we don't handle:
        if ( idt == Comment || idt == Other )
        {
            // Skip to the next line.
            m_iPos += nextNewline(m_pCur, m_pFinal);

            // If i is -1, we've reached the end of the data.
            if ( m_iPos < 0 )
                break;

            // Otherwise, i will be incremented to point to the first character of the next line.
        }
        else if ( idt == Position )
        {
            if ( !process<QVector3D>(m_LocalPositions, 3) )
                return m_Result;
        }
        else if ( idt == Normal )
        {
            if ( !process<QVector3D>(m_LocalNormals, 3) )
                return m_Result;
        }
        else if ( idt == UV )
        {
            if ( !process<QVector2D>(m_LocalUVs, 2) )
                return m_Result;
        }
        else if ( idt == Face )
        {
            if ( !processFace() )
                return m_Result;
        }
    }

    consolidate(positions, normals, uvs, indices);

    // Reaching here means everything was OK.
    return m_Result;
}

template<typename T>
bool ObjFileParser::process(QList<T> &list, int floatCount)
{
    // This should never be 0 because a newline is not a position token!
    int newlineOffset = nextNewline(m_pCur, m_pFinal);
    if ( newlineOffset < 0 )
    {
        newlineOffset = m_pArray->length()-1;
    }

    // We don't pass in the newline character itself.
    int advance = parseFloats<T>(QByteArray(m_pCur, newlineOffset), list, floatCount, m_Result.error);

    if ( m_Result.error != ObjFileParser::NoError )
    {
        m_Result.errorPosition = m_iPos + advance;
        return false;
    }

    // If the return value was < 0, that means we can progress to the next line.
    if ( advance < 0 )
    {
        m_iPos += newlineOffset;
    }
    else
    {
        m_iPos += advance;
    }

    return true;
}

bool ObjFileParser::processFace()
{
    // This should never be 0 because a newline is not a position token!
    int newlineOffset = nextNewline(m_pCur, m_pFinal);
    if ( newlineOffset < 0 )
    {
        newlineOffset = m_pArray->length()-1;
    }

    QList<FaceVertex> list;
    int advance = parseFace(QByteArray(m_pCur, newlineOffset), list, m_Result.error);

    if ( m_Result.error != ObjFileParser::NoError )
    {
        m_Result.errorPosition = m_iPos + advance;
        return false;
    }

    // Create triangles.
    for ( int i = 0; i < list.count(); i++ )
    {
        if ( i > 2 )
        {
            m_FaceVertices.append(list.at(0));
            m_FaceVertices.append(list.at(i-1));
        }

        m_FaceVertices.append(list.at(i));
    }

    // If the return value was < 0, that means we can progress to the next line.
    if ( advance < 0 )
    {
        m_iPos += newlineOffset;
    }
    else
    {
        m_iPos += advance;
    }

    return true;
}

void ObjFileParser::consolidate(QList<QVector3D> &positions, QList<QVector3D> &normals, QList<QVector2D> &uvs, QList<unsigned int> &indices)
{
    m_IndexTable.clear();

    for ( int i = 0; i < m_FaceVertices.count(); i++ )
    {
        const FaceVertex &fv = m_FaceVertices.at(i);

        int index = -1;
        // If we've encountered a vertex like this before:
        if ( m_IndexTable.contains(fv) )
        {
            // It's at this index in the arrays.
            index = m_IndexTable.value(fv);
        }
        // Otherwise, add the data to the arrays.
        else
        {
            Q_ASSERT(positions.count() == normals.count() && normals.count() == uvs.count());
            index = positions.count();
            m_IndexTable.insert(fv, index);

            int pos = fv.atts[FACEATT_POSITION]-1;
            positions.append(pos >= 0 ? m_LocalPositions.at(pos) : QVector3D());

            int nrm = fv.atts[FACEATT_NORMAL]-1;
            normals.append(nrm >= 0 ? m_LocalNormals.at(nrm) : QVector3D());

            int uv = fv.atts[FACEATT_UV]-1;
            uvs.append(uv >= 0 ? m_LocalUVs.at(uv) : QVector2D());
        }

        Q_ASSERT(index >= 0);
        indices.append(index);
    }
}
