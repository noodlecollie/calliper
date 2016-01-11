#ifndef OBJFILEPARSER_H
#define OBJFILEPARSER_H

#include <QList>
#include <QVector3D>
#include <QVector2D>
#include <QByteArray>
#include <QHash>

class ObjFileParser
{
public:
    ObjFileParser();

    enum ParseError
    {
        NoError = 0,
        FileAccessError,     // Could not read one or more input files.
        InvalidNumber,      // A number could not be interpreted from a string.
        InvalidLineFormat,  // A line did not contain the correct information to be interpreted.
        InvalidFace,        // A face did not contain enough vertices to be valid.
        InvalidVertexRef,   // A face's vertex reference could not be parsed.
    };

    struct ParseResult
    {
        ParseError error;
        int errorPosition;
    };

    class FaceVertex
    {
    public:
        FaceVertex() { memset(atts, 0, 3*sizeof(int)); }

        int atts[3];

        bool operator==(const FaceVertex &other) const
        {
            return atts[0] == other.atts[0] &&
                    atts[1] == other.atts[1] &&
                    atts[2] == other.atts[2];
        }

        bool operator!=(const FaceVertex &other) const
        {
            return !(*this == other);
        }

        bool isValid() const
        {
            return atts[0] > 0 && atts[1] > 0 && atts[2] > 0;
        }
    };

    // Any \r characters must be removed.
    ParseResult fillAttributes(const QString &filename, QList<QVector3D> &positions, QList<QVector3D> &normals, QList<QVector2D> &uvs,
                       QList<unsigned int> &indices);

    static QString errorString(const ParseResult &result);

private:
    enum IdentifierToken
    {
        Position = 0,
        Normal,
        UV,
        Face,
        Comment,

        Other = -1
    };

    template<typename T>
    bool process(QList<T> &list, int floatCount);
    bool processFace();

    static IdentifierToken shortIdentifier(const char* id);
    static IdentifierToken getNextIdentifierToken(const char* begin, const char* final);
    static void parseFaceVertex(const QByteArray &token, ObjFileParser::FaceVertex &fv);
    static int parseFace(const QByteArray &line, QList<FaceVertex> &fvList, ObjFileParser::ParseError &error);
    void consolidate(QList<QVector3D> &positions, QList<QVector3D> &normals, QList<QVector2D> &uvs, QList<unsigned int> &indices);

    int m_iPos;
    ParseResult m_Result;
    const char* m_pCur;
    const char* m_pFinal;
    QByteArray m_Array;
    QHash<FaceVertex,int>  m_IndexTable;

    QList<QVector3D>    m_LocalPositions;
    QList<QVector3D>    m_LocalNormals;
    QList<QVector2D>    m_LocalUVs;
    QList<FaceVertex>   m_FaceVertices;
};

#endif // OBJFILEPARSER_H
