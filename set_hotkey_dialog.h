#ifndef SET_HOTKEY_DIALOG_H
#define SET_HOTKEY_DIALOG_H

#include <QDialog>

namespace Ui {
    class SetHotKeyDialog;
}

class SetHotKeyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetHotKeyDialog(QWidget *parent = 0);
    ~SetHotKeyDialog();

private:
    Ui::SetHotKeyDialog *ui;
};

#endif // SET_HOTKEY_DIALOG_H
