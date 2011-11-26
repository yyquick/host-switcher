#include "share_config_dialog.h"
#include "ui_share_config_dialog.h"

ShareConfigDialog::ShareConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShareConfigDialog)
{
    ui->setupUi(this);
}

ShareConfigDialog::~ShareConfigDialog()
{
    delete ui;
}
