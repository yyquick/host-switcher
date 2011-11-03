#include "hotkey_edit.h"

HotkeyEdit::HotkeyEdit(QWidget *parent) :
    QLineEdit(parent)
{
	this->setReadOnly(true);
}

void HotkeyEdit::keyPressEvent(QKeyEvent *event)
{
	if (event->text() != "") {
		QKeySequence ks(event->modifiers() + event->key());
		this->setText(ks.toString());
	}
}
