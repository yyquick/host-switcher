#include "load_preview_dialog.h"
#include "ui_load_preview_dialog.h"

LoadPreviewDialog::LoadPreviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadPreviewDialog)
{
    ui->setupUi(this);

	host_config_ = new HostConfig();
}

LoadPreviewDialog::~LoadPreviewDialog()
{
    delete ui;
}

void LoadPreviewDialog::showMyself(QString content)
{
	this->clearMyself();

	host_config_->import_config_content('', content);
	ui->itemListTableWidget->setColumnCount(1);
	ui->itemListTableWidget->setRowCount(host_config_->section_list_.count());
	HostConfig::SectionListIter iter;
	int i = 0;
	for (iter = host_config_->section_list_.begin(); iter != host_config_->section_list_.end(); iter++) {
		QTableWidgetItem *column = new QTableWidgetItem(iter->name_);
		if (iter->is_enable_) {
			column->setCheckState(Qt::Checked);
		} else {
			column->setCheckState(Qt::Unchecked);
		}
		if (iter == host_config_->section_list_.begin()) {
			column->setBackgroundColor(QColor(200, 200, 200));
			column->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		}
		ui->itemListTableWidget->setItem(i, 0, column);
		i++;
	}
}

void LoadPreviewDialog::clearMyself()
{
	ui->itemListTableWidget->clear();
	host_config_->section_list_.clear();
}
