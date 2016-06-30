#include "keyvaluestoken.h"
#include <QString>

#define DELIM_QUOTE '\"'
#define DELIM_PUSH '{'
#define DELIM_POP '}'
#define DELIM_PREPROCESSOR '#'

namespace
{
    const QString DELIM_LINECOMMENT("//");
    const QString DELIM_BLOCKCOMMENT_BEGIN("/*");
    const QString DELIM_BLOCKCOMMENT_END("*/");
}

KeyValuesToken::KeyValuesToken()
{
    m_iTokenType = TokenInvalid;
    m_iBeginPos = -1;
    m_iLength = 0;
}

KeyValuesToken::KeyValuesToken(const QByteArray &kv, int position)
{
    m_iTokenType = getToken(kv, position);
    m_iBeginPos = position;
    m_iLength = lengthOfToken(kv, position, m_iTokenType);
}

bool KeyValuesToken::is(TokenType token) const
{
    return m_iTokenType == token;
}

KeyValuesToken::TokenType KeyValuesToken::type() const
{
    return m_iTokenType;
}

bool KeyValuesToken::isValid() const
{
    return m_iTokenType != TokenInvalid;
}

// Assumes the position is valid.
KeyValuesToken::TokenType KeyValuesToken::getToken(const QByteArray &arr, int position)
{
    TokenType token = TokenInvalid;

    // Try just the first character.
    if ( getTokenFromFirstChar(arr.at(position), token) )
        return token;

    if ( position == arr.length() - 1 )
        return TokenInvalid;

    char str[3];
    str[0] = arr.at(position);
    str[1] = arr.at(position+1);
    str[2] = '\0';

    // Now try two characters.
    if ( getTokenFromFirstTwoChars(str, token) )
        return token;

    return TokenInvalid;
}

// Returns true if the first character was enough to determine the token.
bool KeyValuesToken::getTokenFromFirstChar(char ch, TokenType &token)
{
    switch (ch)
    {
        case DELIM_QUOTE:
            token = TokenStringQuoted;
            return true;

        case DELIM_PUSH:
            token = TokenPush;
            return true;

        case DELIM_POP:
            token = TokenPop;
            return true;

        case DELIM_PREPROCESSOR:
            token = TokenPreprocessor;
            return true;

        default:
            break;
    }

    // Check if it's alphanumeric.
    if ( isAlphaNumeric(ch) )
    {
        token = TokenStringUnquoted;
        return true;
    }

    return false;
}

// Returns true if the first two characters were enough to determine the token.
// Assumes str is a null-terminated two-character string.
bool KeyValuesToken::getTokenFromFirstTwoChars(const char *str, TokenType &token)
{
    if ( str == DELIM_LINECOMMENT )
    {
        token = TokenLineComment;
        return true;
    }

    if ( str == DELIM_BLOCKCOMMENT_BEGIN )
    {
        token = TokenBlockComment;
        return true;
    }

    return false;
}

bool KeyValuesToken::isWhitespace(char ch)
{
    switch (ch)
    {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            return true;

        default:
            return false;
    }
}

bool KeyValuesToken::isAlphaNumeric(char ch)
{
    return (ch >= 'A' && ch <= 'Z') ||
           (ch >= 'a' && ch <= 'z') ||
           (ch >= '0' && ch <= '9') ||
           ch == '_';
}

// Position is the beginning position of the first delimiter.
// Offset is how long the first delimiter is, in chars.
// The length returned is all of the characters in the token,
// including the beginning and ending delimiters.
// A final delimeter MUST be found - if it is not, a length
// of 0 is returned.
int KeyValuesToken::lengthOfToken(const QByteArray &arr, int position, int offset, const QByteArray &endDelim)
{
    if ( position >= arr.length() )
        return 0;

    int index = arr.indexOf(endDelim, position+offset);
    if ( index < 0 )
        return 0;

    return index - position + 1;
}

int KeyValuesToken::lengthOfToken(const QByteArray &arr, int position, TokenType type)
{
    switch (type)
    {
        case TokenInvalid:
        case TokenPush:
        case TokenPop:
        case TokenPreprocessor:
            return 1;

        // Length is entire quoted string.
        case TokenStringQuoted:
            return lengthOfToken(arr, position, 1, "\"");

        // Length is entire comment including trailing newline.
        case TokenLineComment:
            return lengthOfToken(arr, position, 2, "\n");

        // Length is entire comment including end tag.
        case TokenBlockComment:
            return lengthOfToken(arr, position, 2, DELIM_BLOCKCOMMENT_END.toLatin1());

        case TokenStringUnquoted:
        default:
            break;
    }

    // If it's an unquoted string, we need to find the next non-alphanumeric character.
    if ( type == TokenStringUnquoted )
    {
        int next = position+1;
        while ( next < arr.length() )
        {
            if ( !isAlphaNumeric(arr.at(next)) )
                break;

            next++;
        }

        // Length is all the characters that were alphanumeric.
        return next - position;
    }

    return 0;
}

bool KeyValuesToken::isString() const
{
    return is(TokenStringUnquoted) || is(TokenStringQuoted);
}

void KeyValuesToken::writeJson(const QByteArray &input, QByteArray &json, int prefix)
{
    // Don't write token types we don't care about.
    if ( !shouldWriteJson() )
        return;

    // If we're an unquoted string, enclose in quotes first.
    if ( is(TokenStringUnquoted) )
    {
        json.append('\"');
    }

    if ( prefix >= 0 && isString() )
    {
        json.append(QString("%0_").arg(prefix));
    }

    json.append(input.constData() + m_iBeginPos, m_iLength);

    if ( is(TokenStringUnquoted) )
    {
        json.append('\"');
    }
}

bool KeyValuesToken::shouldWriteJson() const
{
    return is(TokenStringQuoted) || is(TokenStringUnquoted) ||
            is(TokenPush) || is(TokenPop);
}

int KeyValuesToken::length() const
{
    return m_iLength;
}

int KeyValuesToken::offset() const
{
    return m_iBeginPos;
}
