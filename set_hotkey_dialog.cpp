#include "set_hotkey_dialog.h"
#include "ui_set_hotkey_dialog.h"

SetHotkeyDialog::SetHotkeyDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::SetHotkeyDialog)
{
    ui->setupUi(this);

	parentWidget = (HostSwitcher *)parent;
}

SetHotkeyDialog::~SetHotkeyDialog()
{
    delete ui;
}

void SetHotkeyDialog::save()
{

}
