#ifndef VIEWPORTUSEROPTIONS_H
#define VIEWPORTUSEROPTIONS_H

#include <QWidget>

namespace Ui {
class ViewportUserOptions;
}

class ViewportUserOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ViewportUserOptions(QWidget *parent = 0);
    ~ViewportUserOptions();

signals:
    void focusHighlightStatusChanged(bool);

public slots:
    void toggleVisibility();

private:
    Ui::ViewportUserOptions *ui;
};

#endif // VIEWPORTUSEROPTIONS_H
