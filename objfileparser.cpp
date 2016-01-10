#include "objfileparser.h"

#define IDSHORT_POSITION    (('v' << 8) | 0)
#define IDSHORT_NORMAL      (('v' << 8) | 'n')
#define IDSHORT_UV          (('v' << 8) | 't')
#define IDSHORT_FACE        (('f' << 8) | 0)
#define IDSHORT_COMMENT     (('#' << 8) | 0)
#define MAX_IDENTIFIER_LENGTH   2

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

ObjFileParser::ObjFileParser()
{
}

ObjFileParser::ParseResult ObjFileParser::fillAttributes(const QByteArray &arr, QList<QVector3D> &positions, QList<QVector3D> &normals, QList<QVector2D> &uvs,
                    QList<unsigned int> &indices)
{
    m_pArray = &arr;
    const char* base = arr.constData();
    m_pFinal = base + (arr.count()-1);
    memset(&m_Result, 0, sizeof(ObjFileParser::ParseResult));

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
            if ( !process<QVector3D>(positions, 3) )
                return m_Result;
        }
        else if ( idt == Normal )
        {
            if ( !process<QVector3D>(normals, 3) )
                return m_Result;
        }
        else if ( idt == UV )
        {
            if ( !process<QVector2D>(uvs, 2) )
                return m_Result;
        }
    }

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
