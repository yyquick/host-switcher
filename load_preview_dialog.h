#ifndef LOAD_PREVIEW_DIALOG_H
#define LOAD_PREVIEW_DIALOG_H

#include <QDialog>
#include <QString>
#include "host_config.h"

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

private:
    Ui::LoadPreviewDialog *ui;

	void clearMyself();
};

#endif // LOAD_PREVIEW_DIALOG_H
