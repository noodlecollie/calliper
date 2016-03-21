#ifndef TEXTUREPLANE_H
#define TEXTUREPLANE_H

#include <QObject>
#include <QString>
#include <QVector3D>
#include <QVector2D>
#include "callipermath.h"
#include "iserialisable.h"

class TexturePlane : public QObject, public ISerialisable
{
    Q_OBJECT
public:
    explicit TexturePlane(QObject* parent = 0);
    TexturePlane(const QJsonObject &serialisedData, QObject* parent = 0);

    TexturePlane* clone() const;

    // Texture to be used on the face.
    QString texturePath() const;
    void setTexturePath(const QString &path);

    // The U and V axes specify the U and V axes of the texture in 3D space.
    // The length of each axis specifies how many texture units there are per world unit.
    // Larger values make the texture appear more squashed in that dimension.
    // The U and V axes correspond to OpenGL texture axes - ie. U is right, V is up.
    // NOTE: The U and V axes are always returned as unit vectors and are not affected
    // by the scale value, because this is how Hammer deals with them.
    // When calculating projections of 3D points onto the plane, the scale value is taken
    // into account separately.
    QVector3D uAxis() const;
    QVector3D vAxis() const;

    // The scale specifies how many world units there are per texture unit; it is
    // the reciprocal of the U/V axes. A larger scale makes the texture appear
    // more stretched in that dimension.
    QVector2D scale() const;
    void setScale(const QVector2D &sc);

    // The translation specifies the location of the plane origin on the texture.
    // Texture axes begin at the top left; X extends right and Y down.
    // Hence, positive X translations move the origin to the right on the texture,
    // so the texture appears to scroll left; positive Y translations move the origin
    // down the texture so it appears to scroll up.
    QVector2D translation() const;
    void setTranslation(const QVector2D &tr);

    // The rotation specifies the rotation of the texture around the plane origin.
    // When looking along the plane normal, a positive rotation rotates the texture
    // clockwise along the normal; when looking at the texture surface, this means
    // it appears to rotate anticlockwise.
    float rotation() const;
    void setRotation(float rot);

    // Returns the default U and V vectors for a plane with a normal along the given positive axis.
    static void uvAxes(Math::AxisIdentifier axis, QVector3D &uAxis, QVector3D &vAxis);

    // Returns the U and V vectors for the plane, with the provided normal.
    // U and V are unit vectors rotated correctly according to the texture rotation.
    void uvAxes(const QVector3D &normal, QVector3D &uAxis, QVector3D &vAxis) const;

    // Maps a 3D point to a 2D OpenGL texture co-ordinate.
    // The point is assumed to be in the same co-ordinate space as the plane.
    // U and V should be unit vectors.
    QVector2D textureCoordinate(const QVector3D &point, const QSize &textureSize, const QVector3D &uAxis,
                                const QVector3D &vAxis) const;

    // Same as above but auto-generates U and V from the given normal.
    QVector2D textureCoordinate(const QVector3D &point, const QSize &textureSize, const QVector3D &normal) const;

    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;

signals:
    void dataChanged();

protected:
    explicit TexturePlane(const TexturePlane &cloneFrom);

private:
    void initDefaults();

    QString     m_szTexturePath;
    QVector2D   m_vecScale;
    QVector2D   m_vecTranslation;
    float       m_flRotation;
};

#endif // TEXTUREPLANE_H
