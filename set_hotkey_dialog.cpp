#include "set_hotkey_dialog.h"
#include "ui_set_hotkey_dialog.h"

SetHotkeyDialog::SetHotkeyDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::SetHotkeyDialog)
{
    ui->setupUi(this);

	parentWidget = (HostSwitcher *)parent;

	connect(this->ui->saveHotkeyButton, SIGNAL(clicked()), this, SLOT(save()));
	connect(this->ui->cancelHotkeyButton, SIGNAL(clicked()), this, SLOT(reject()));
}

SetHotkeyDialog::~SetHotkeyDialog()
{
    delete ui;
}

void SetHotkeyDialog::save()
{
	if (parentWidget->scRestore) {
		disconnect(parentWidget->scRestore, 0, 0, 0);
		delete parentWidget->scRestore;
	}
	if (parentWidget->scSwitchUp) {
		disconnect(parentWidget->scSwitchUp, 0, 0, 0);
		delete parentWidget->scSwitchUp;
	}
	if (parentWidget->scSwitchDown) {
		disconnect(parentWidget->scSwitchDown, 0, 0, 0);
		delete parentWidget->scSwitchDown;
	}

	parentWidget->host_config_->config_[HostConfig::HOTKEY_RESTORE_KEY] = ui->restoreHotKeyEdit->text();
	parentWidget->host_config_->config_[HostConfig::HOTKEY_MOVE_UP_KEY] = ui->moveUpHotkeyEdit->text();
	parentWidget->host_config_->config_[HostConfig::HOTKEY_MOVE_DOWN_KEY] = ui->moveDownHotkeyEdit->text();

	parentWidget->bindHotkeys();
	parentWidget->host_config_->save_info();

	this->hide();
}

void SetHotkeyDialog::showMyself()
{
	QString hotkeyMoveUp = parentWidget->host_config_->config_[HostConfig::HOTKEY_MOVE_UP_KEY];
	QString hotkeyMoveDown = parentWidget->host_config_->config_[HostConfig::HOTKEY_MOVE_DOWN_KEY];
	QString hotkeyRestore = parentWidget->host_config_->config_[HostConfig::HOTKEY_RESTORE_KEY];

	ui->moveUpHotkeyEdit->setText(hotkeyMoveUp);
	ui->moveDownHotkeyEdit->setText(hotkeyMoveDown);
	ui->restoreHotKeyEdit->setText(hotkeyRestore);

	this->show();
}
