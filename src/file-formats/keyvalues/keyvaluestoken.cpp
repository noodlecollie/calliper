#include "keyvaluestoken.h"
#include <QString>

namespace FileFormats
{
    namespace
    {
        const char DELIM_QUOTE = '\"';
        const char DELIM_PUSH = '{';
        const char DELIM_POP = '}';
        const char DELIM_PREPROCESSOR = '#';

        const QByteArray DELIM_LINECOMMENT("//");
        const QByteArray DELIM_BLOCKCOMMENT_BEGIN("/*");
        const QByteArray DELIM_BLOCKCOMMENT_END("*/");

        bool isSingleCharDelimeter(char ch)
        {
            switch (ch)
            {
                case DELIM_QUOTE:
                case DELIM_PUSH:
                case DELIM_POP:
                case DELIM_PREPROCESSOR:
                    return true;

                default:
                    return false;
            }
        }

        bool isTwoCharacterDelimeter(const QByteArray& array, int position)
        {
            if ( position >= array.length() - 1 )
                return false;

            QByteArray testString = array.mid(position, 2);
            return testString == DELIM_LINECOMMENT ||
                    testString == DELIM_BLOCKCOMMENT_BEGIN ||
                    testString == DELIM_BLOCKCOMMENT_END;
        }

        bool isValidForNonQuotedString(const QByteArray& array, int position)
        {
            // Non-quoted strings are allowed to contain ASCII text (ie. not
            // control characters) that is NOT also a delimeter.

            char ch = array.at(position);

            // No control chars or whitespace.
            if ( ch < 32 || ch > 126 || KeyValuesToken::isWhitespace(ch) )
                return false;

            return !isSingleCharDelimeter(ch) && !isTwoCharacterDelimeter(array, position);
        }
    }

    QString KeyValuesToken::tokenReadableName(TokenType type)
    {
        switch ( type )
        {
            case TokenInvalid:
                return "Invalid";

            case TokenStringQuoted:
                return "Quoted String";

            case TokenStringUnquoted:
                return "Unquoted String";

            case TokenPush:
                return "Group Begin";

            case TokenPop:
                return "Group End";

            case TokenLineComment:
                return "Line Comment";

            case TokenBlockComment:
                return "Block Comment";

            case TokenPreprocessor:
                return "Preprocessor Instruction";

            default:
                return "Unknown";
        }
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

    QString KeyValuesToken::readableName() const
    {
        return tokenReadableName(type());
    }

    bool KeyValuesToken::isValid() const
    {
        return m_iTokenType != TokenInvalid;
    }

    bool KeyValuesToken::isIncomplete() const
    {
        return isValid() && m_iLength < 1;
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

    // Assumes the position is valid.
    KeyValuesToken::TokenType KeyValuesToken::getToken(const QByteArray &arr, int position)
    {
        TokenType token = TokenInvalid;

        // Try just the first character.
        if ( getTokenFromFirstChar(arr, position, token) )
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
    bool KeyValuesToken::getTokenFromFirstChar(const QByteArray &arr, int position, TokenType &token)
    {
        switch (arr.at(position))
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

        if ( isValidForNonQuotedString(arr, position) )
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

    // Position is the beginning position of the first delimiter.
    // Offset is how long the first delimiter is, in chars.
    // The length returned is all of the characters in the token,
    // including the beginning and ending delimiters.
    int KeyValuesToken::lengthOfToken(const QByteArray &arr, int position, int offset, const QByteArray &endDelim,
                                      bool eofIsDelimeter)
    {
        if ( position >= arr.length() )
            return 0;

        int index = arr.indexOf(endDelim, position+offset);
        if ( index < 0 )
        {
            if ( eofIsDelimeter )
                return arr.length() - position;
            else
                return 0;
        }

        return index - position + endDelim.length();
    }

    int KeyValuesToken::lengthOfToken(const QByteArray &arr, int position, TokenType type)
    {
        switch (type)
        {
            case TokenInvalid:
            case TokenPush:
            case TokenPop:
                return 1;

            // Length is entire line including trailing newline (or EOF).
            case TokenPreprocessor:
                return lengthOfToken(arr, position, 1, "\n", true);

            // Length is entire quoted string.
            case TokenStringQuoted:
                return lengthOfToken(arr, position, 1, "\"");

            // Length is entire comment including trailing newline (or EOF).
            case TokenLineComment:
                return lengthOfToken(arr, position, 2, "\n", true);

            // Length is entire comment including end tag.
            case TokenBlockComment:
                return lengthOfToken(arr, position, 2, DELIM_BLOCKCOMMENT_END);

            case TokenStringUnquoted:
            default:
                break;
        }

        // If it's an unquoted string, we need to find the next invalid character.
        if ( type == TokenStringUnquoted )
        {
            int next = position+1;
            while ( next < arr.length() )
            {
                if ( !isValidForNonQuotedString(arr, next) )
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
        if ( isString() )
        {
            json.append('\"');
        }

        if ( prefix >= 0 )
        {
            json.append(QString("%0_").arg(prefix));
        }

        json.append(getString(input).replace("\\", "\\\\"));

        if ( isString() )
        {
            json.append('\"');
        }
    }

    QByteArray KeyValuesToken::getString(const QByteArray &input)
    {
        if ( is(TokenStringQuoted) )
        {
            return input.mid(m_iBeginPos + 1, m_iLength - 2);
        }
        else
        {
            return input.mid(m_iBeginPos, m_iLength);
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
}
