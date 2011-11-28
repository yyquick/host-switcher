#include "share_config_dialog.h"
#include "ui_share_config_dialog.h"
#include "host_config.h"

ShareConfigDialog::ShareConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShareConfigDialog)
{
	lock = false;
	this->parent = (HostSwitcher *)parent;
    ui->setupUi(this);

	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
}

ShareConfigDialog::~ShareConfigDialog()
{
    delete ui;
}

void ShareConfigDialog::show() {
	lock = true;
	ui->configItemTableWidget->clear();
	ui->configItemTableWidget->setColumnCount(1);
	ui->configItemTableWidget->setRowCount(parent->host_config_->section_list_.count());
	HostConfig::SectionListIter iter;
	int i = 0;
	for (iter = parent->host_config_->section_list_.begin(); iter != parent->host_config_->section_list_.end(); iter++) {
		QTableWidgetItem *column = new QTableWidgetItem(iter->name_);
		if (iter->share_) {
			column->setCheckState(Qt::Checked);
		} else {
			column->setCheckState(Qt::Unchecked);
		}
		ui->configItemTableWidget->setItem(i, 0, column);
		i++;
	}
	updateCheckBox();
	lock = false;
	QDialog::show();
}

void ShareConfigDialog::save()
{
	int rowCount = ui->configItemTableWidget->rowCount();
	int i;
	QTableWidgetItem *item;
	for (i = 0; i < rowCount; i++) {
		item = ui->configItemTableWidget->item(i, 0);
		if (item->checkState() == Qt::Checked) {
			parent->host_config_->section_list_[i].share_ = true;
		} else {
			parent->host_config_->section_list_[i].share_ = false;
		}
	}
	parent->host_config_->save_info();
	hide();
}

void ShareConfigDialog::on_configItemTableWidget_itemChanged(QTableWidgetItem *changedItem)
{
	if (lock) {
		return;
	}
	updateCheckBox();
}

void ShareConfigDialog::on_selectAllCheckBox_clicked()
{
	lock = true;
	Qt::CheckState state;
	if (ui->selectAllCheckBox->checkState() == Qt::Checked) {
		state = Qt::Checked;
	} else if (ui->selectAllCheckBox->checkState() == Qt::PartiallyChecked) {
		state = Qt::Checked;
	} else {
		state = Qt::Unchecked;
	}
	ui->selectAllCheckBox->setCheckState(state);
	int rowCount = ui->configItemTableWidget->rowCount();
	int i = 0;
	QTableWidgetItem *item;
	for (i = 0; i < rowCount; i++) {
		item = ui->configItemTableWidget->item(i, 0);
		item->setCheckState(state);
	}
	lock = false;
}

void ShareConfigDialog::updateCheckBox()
{
	int rowCount = ui->configItemTableWidget->rowCount();
	int i, c = 0;
	QTableWidgetItem *item;
	for (i = 0; i < rowCount; i++) {
		item = ui->configItemTableWidget->item(i, 0);
		if (item->checkState() == Qt::Checked) {
			c++;
		}
	}
	if (c == rowCount) {
		ui->selectAllCheckBox->setCheckState(Qt::Checked);
	} else if (c == 0) {
		ui->selectAllCheckBox->setCheckState(Qt::Unchecked);
	} else {
		ui->selectAllCheckBox->setCheckState(Qt::PartiallyChecked);
	}
}
