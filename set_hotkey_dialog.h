#ifndef SET_HOTKEY_DIALOG_H
#define SET_HOTKEY_DIALOG_H

#include <QDialog>
#include "host_switcher.h"

namespace Ui {
	class SetHotkeyDialog;
}

class SetHotkeyDialog : public QDialog
{
    Q_OBJECT

	void showMyself();

public:
	explicit SetHotkeyDialog(QWidget *parent = 0);
	~SetHotkeyDialog();

private:
	Ui::SetHotkeyDialog *ui;

	HostSwitcher *parentWidget;

private slots:
	void save();
};

#endif // SET_HOTKEY_DIALOG_H
