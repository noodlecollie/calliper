#include "objfileparser.h"

namespace ObjFileParser
{
    enum IdentifierToken
    {
        Position = 0,
        Normal,
        UV,
        Face,
        Comment,

        Other = -1
    };

    #define IDSHORT_POSITION    (('v' << 8) | 0)
    #define IDSHORT_NORMAL      (('v' << 8) | 'n')
    #define IDSHORT_UV          (('v' << 8) | 't')
    #define IDSHORT_FACE        (('f' << 8) | 0)
    #define IDSHORT_COMMENT     (('#' << 8) | 0)
    #define MAX_IDENTIFIER_LENGTH   2

    // Potentially faster? Assumes an array of length at least 2,
    // and where any chars after the last valid char are 0.
    static IdentifierToken shortIdentifier(const char* id)
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
    static IdentifierToken getNextIdentifierToken(const char* begin, const char* final)
    {
        const int idLen = MAX_IDENTIFIER_LENGTH + 1;
        char identifier[idLen];
        memset(identifier, 0, idLen*sizeof(char));

        for (int i = 0; i < idLen-1 && (begin+i) <= final; i++)
        {
            if ( isWhiteSpace(begin+i) )
                break;

            identifier[i] = *begin;
        }

        return shortIdentifier(identifier);
    }

    // We are given the final character so that we don't go over it.
    static int nextNewline(const char* begin, const char* final)
    {
        int advance = 0;
        while ( begin != final )
        {
            if ( *begin == '\n' )
            {
                // If the next character is \r, return that instead.
                // Otherwise, return this character.
                return *(begin+1) == '\r' ? (advance+1) : advance;
            }

            advance++;
            begin++;
        }

        // We've reached the final character - return whether or not this is a newline.
        return (*final == '\n') ? advance : -1;
    }

    static int parsePosition(const char* begin, const char* final, QList<QVector3D> &positions, ObjFileParser::ParseError &error)
    {
        const char* c = begin;

        // Wind past the current token.
        while ( !isWhiteSpace(c) && c < final )
        {
            c++;
        }

        if ( c == final )
        {
            error = UnexpectedEOF;
            return final - begin;
        }

        // Parse the float values.
        char buffer[3*16];
        QVector3D pos;
        char* xyz[3] = { &(buffer[0*16]), &(buffer[1*16]), &(buffer[2*16]) };
        memset(buffer, 0, 3*16);

        // For each axis:
        for ( int i = 0; i < 3; i++ )
        {
            // Find the start of the number.
            while ( isWhiteSpace(c) && c <= final )
            {
                c++;
            }

            // If there is no number, return an error.
            if ( c > final )
            {
                error = UnexpectedEOF;
                return final - begin;
            }

            const char* start = c;

            // Copy the string into the appropriate buffer.
            for ( int j = 0; j < 15; j++ )
            {
                if ( c > final || isWhiteSpace(c) || *c == '\n' )
                {
                    break;
                }

                xyz[i][j] = *c;
                c++;
            }

            // If we passed the final pointer before completing all of the copying, return an error.
            if ( i < 2 && c > final )
            {
                error = UnexpectedEOF;
                return final - begin;
            }

            QString str(xyz[i]);
            bool ok = false;
            float value = str.toFloat(&ok);
            if ( !ok )
            {
                error = InvalidNumber;
                return start - begin;
            }

            switch (i)
            {
                case 0:
                    pos.setX(value);
                    break;

                case 1:
                    pos.setY(value);
                    break;

                case 2:
                    pos.setZ(value);
                    break;

                default:
                    break;
            }
        }

        positions.append(pos);
        return c <= final ? (c - begin) : (final - begin);
    }

    ParseResult fillAttributes(const QByteArray &arr, QList<QVector3D> &positions, QList<QVector3D> &normals, QList<QVector2D> &uvs,
                        QList<unsigned int> &indices)
    {
        const char* base = arr.constData();
        const char* final = base + (arr.count()-1);
        ParseResult result;
        memset(&result, 0, sizeof(ParseResult));

        for ( int i = 0; i < arr.count(); i++ )
        {
            const char* c = base + i;

            // Find the first non-whitespace character.
            if ( isWhiteSpace(c) || *c == '\n' )
                continue;

            // Get an identifier for the token.
            IdentifierToken idt = getNextIdentifierToken(c, final);

            // If the identifier is a comment, or something else we don't handle:
            if ( idt == Comment || idt == Other )
            {
                // Skip to the next line.
                i += nextNewline(c, final);

                // If i is -1, we've reached the end of the data.
                if ( i < 0 )
                    break;

                // Otherwise, i will be incremented to point to the first character of the next line.
            }
            else if ( idt == Position )
            {
                i += parsePosition(c, final, positions, result.error);
                if ( result.error != NoError )
                {
                    result.errorPosition = i;
                    return result;
                }
            }

        }

        // Reaching here means everything was OK.
        return result;
    }
}

