#ifndef OBJFILEPARSER_H
#define OBJFILEPARSER_H

#include <QList>
#include <QVector3D>
#include <QVector2D>
#include <QByteArray>

class ObjFileParser
{
public:
    ObjFileParser();

    enum ParseError
    {
        NoError = 0,
        UnexpectedEOF,      // The file ended unexpectedly.
        InvalidNumber,      // A number could not be interpreted from a string.
        InvalidLineFormat,  // A line did not contain the correct information to be interpreted.
    };

    struct ParseResult
    {
        ParseError error;
        int errorPosition;
    };

    // Any \r characters must be removed.
    ParseResult fillAttributes(const QByteArray &arr, QList<QVector3D> &positions, QList<QVector3D> &normals, QList<QVector2D> &uvs,
                       QList<unsigned int> &indices);

private:
    ParseResult m_Result;
};

#endif // OBJFILEPARSER_H
