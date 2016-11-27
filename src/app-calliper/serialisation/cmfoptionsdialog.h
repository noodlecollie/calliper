#ifndef CMFOPTIONSDIALOG_H
#define CMFOPTIONSDIALOG_H

#include <QDialog>
#include "callipermapfile.h"

namespace Ui {
class CMFOptionsDialog;
}

class CMFOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMFOptionsDialog(QWidget *parent = 0);
    ~CMFOptionsDialog();

    // The current value of format will be used as the default.
    static bool chooseFileFormat(QWidget* parent, CalliperMapFile::FileFormat &format);

private:
    void setDefaultFormat(CalliperMapFile::FileFormat format);
    CalliperMapFile::FileFormat selectedFormat() const;

    Ui::CMFOptionsDialog *ui;
};

#endif // CMFOPTIONSDIALOG_H
