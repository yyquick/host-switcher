#include "set_hotkey_dialog.h"
#include "ui_set_hotkey_dialog.h"

SetHotKeyDialog::SetHotKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetHotKeyDialog)
{
    ui->setupUi(this);
}

SetHotKeyDialog::~SetHotKeyDialog()
{
    delete ui;
}
