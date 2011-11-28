#ifndef SHARE_CONFIG_DIALOG_H
#define SHARE_CONFIG_DIALOG_H

#include <QDialog>
#include "host_switcher.h"

class HostSwitcher;

namespace Ui {
    class ShareConfigDialog;
}

class ShareConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShareConfigDialog(QWidget *parent = 0);
    ~ShareConfigDialog();

private slots:

	void on_selectAllCheckBox_clicked();

	void on_configItemTableWidget_itemChanged(QTableWidgetItem *changedItem);

	void show();

	void save();

private:
    Ui::ShareConfigDialog *ui;
	HostSwitcher *parent;
	bool lock;

	void updateCheckBox();
};

#endif // SHARE_CONFIG_DIALOG_H
