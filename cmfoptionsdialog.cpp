#include "cmfoptionsdialog.h"
#include "ui_cmfoptionsdialog.h"

CMFOptionsDialog::CMFOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMFOptionsDialog)
{
    ui->setupUi(this);
}

CMFOptionsDialog::~CMFOptionsDialog()
{
    delete ui;
}

bool CMFOptionsDialog::chooseFileFormat(QWidget *parent, CalliperMapFile::FileFormat &format)
{
    CMFOptionsDialog dlg(parent);
    dlg.setDefaultFormat(format);

    if ( dlg.exec() != QDialog::Accepted )
        return false;

    format = dlg.selectedFormat();
    return true;
}

void CMFOptionsDialog::setDefaultFormat(CalliperMapFile::FileFormat format)
{
    switch (format)
    {
    case CalliperMapFile::CompactJson:
        ui->rbCompact->setChecked(true);
        break;

    case CalliperMapFile::Binary:
        ui->rbBinary->setChecked(true);
        break;

    default:
        ui->rbStd->setChecked(true);
        break;
    }
}

CalliperMapFile::FileFormat CMFOptionsDialog::selectedFormat() const
{
    if ( ui->rbCompact->isChecked() )
        return CalliperMapFile::CompactJson;

    if ( ui->rbBinary->isChecked() )
        return CalliperMapFile::Binary;

    return CalliperMapFile::IndentedJson;
}
