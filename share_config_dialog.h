#ifndef SHARE_CONFIG_DIALOG_H
#define SHARE_CONFIG_DIALOG_H

#include <QDialog>

namespace Ui {
    class ShareConfigDialog;
}

class ShareConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShareConfigDialog(QWidget *parent = 0);
    ~ShareConfigDialog();

private:
    Ui::ShareConfigDialog *ui;
};

#endif // SHARE_CONFIG_DIALOG_H
