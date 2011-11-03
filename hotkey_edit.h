#ifndef HOTKEY_EDIT_H
#define HOTKEY_EDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class HotkeyEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit HotkeyEdit(QWidget *parent = 0);

protected:
	void keyPressEvent(QKeyEvent *event);

signals:

public slots:

};

#endif // HOTKEY_EDIT_H
