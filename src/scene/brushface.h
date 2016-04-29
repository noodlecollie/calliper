#ifndef BRUSHFACE_H
#define BRUSHFACE_H

#include <QObject>
#include "iserialisable.h"
#include "iraydetectable.h"

class Brush;
class TexturePlane;
class ShaderStack;
class GeometryData;

class BrushFace : public QObject, public ISerialisable, private IRayDetectable
{
    Q_OBJECT
    friend class Brush;
public:
    BrushFace(Brush* parent, QVector<int> vertices = QVector<int>());
    BrushFace(Brush* parent, const QJsonObject &serialisedData);
    virtual ~BrushFace();

    Brush* parentBrush() const;

    int vertexAt(int index) const;
    void appendVertex(int v);
    int vertexCount() const;
    void removeVertex(int index);
    QVector<int> vertexList() const;
    void clearVertices();
    QVector<QVector3D> referencedVertexList() const;

    bool isValid() const;
    QVector3D normal() const;
    float planeDistanceFromOrigin() const;

    TexturePlane* texturePlane() const;

    void draw(ShaderStack *stack);

    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;

private slots:
    void forceRebuildGeometry();

private:
    void initDefaults();
    void notifyVertexRemoved(int index);
    void notifyVertexChanged(int index);
    void buildGeometry();
    virtual float computeIntersection(const Ray3D &ray, QRgb *col = NULL, RayCoordinateSpace space = IRayDetectable::WorldSpace) const;

    QVector<int>      m_Vertices;
    bool            m_bRebuildGeometry;
    TexturePlane*   m_pTexturePlane;
    GeometryData*   m_pGeometry;
};

#endif // BRUSHFACE_H
