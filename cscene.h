#ifndef CSCENE_H
#define CSCENE_H

#include <QWidget>

class CSceneObject;

class CScene : public QWidget
{
    Q_OBJECT
public:
    explicit CScene(QWidget *parent = 0);
    ~CScene();

    CSceneObject* root() const;

signals:

public slots:

private:
    CSceneObject*   m_pRoot;
};

#endif // CSCENE_H
