#ifndef OBJFILEPARSER_H
#define OBJFILEPARSER_H

#include <QList>
#include <QVector3D>
#include <QVector2D>
#include <QByteArray>

namespace ObjFileParser
{
    enum ParseError
    {
        NoError = 0,
        UnexpectedEOF,
        InvalidNumber,
    };

    struct ParseResult
    {
        ParseError error;
        int errorPosition;
    };

    ParseResult fillAttributes(const QByteArray &arr, QList<QVector3D> &positions, QList<QVector3D> &normals, QList<QVector2D> &uvs,
                       QList<unsigned int> &indices);
}

#endif // OBJFILEPARSER_H
