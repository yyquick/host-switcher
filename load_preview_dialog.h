#ifndef LOAD_PREVIEW_DIALOG_H
#define LOAD_PREVIEW_DIALOG_H

#include <QTableWidgetItem>
#include <QDialog>
#include <QString>
#include "host_config.h"
#include "host_switcher.h"

namespace Ui {
    class LoadPreviewDialog;
}

class LoadPreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadPreviewDialog(QWidget *parent = 0);
    ~LoadPreviewDialog();

	HostConfig *host_config_;

	void showMyself(QString content);

public slots:
	void on_itemListTableWidget_itemSelectionChanged();
	void on_itemListTableWidget_itemChanged(QTableWidgetItem *item);

	void saveConfig();
	void on_selectAllBox_stateChanged();

private:
    Ui::LoadPreviewDialog *ui;

	HostSwitcher *parent;

	void clearMyself();
};

#endif // LOAD_PREVIEW_DIALOG_H
