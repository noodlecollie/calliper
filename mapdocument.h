#ifndef MAPDOCUMENT_H
#define MAPDOCUMENT_H

#include <QObject>
#include <QColor>

class Scene;

class MapDocument : public QObject
{
    Q_OBJECT
public:
    explicit MapDocument(QObject* parent = 0);
    ~MapDocument();

    Scene* scene() const;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &col);

private:
    Scene*  m_pScene;
    QColor  m_colBackground;
};

#endif // MAPDOCUMENT_H
