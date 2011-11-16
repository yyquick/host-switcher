#include "load_preview_dialog.h"
#include "ui_load_preview_dialog.h"

LoadPreviewDialog::LoadPreviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadPreviewDialog)
{
	this->parent = (HostSwitcher *)parent;
	ui->setupUi(this);

	host_config_ = new HostConfig();

	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveConfig()));
}

LoadPreviewDialog::~LoadPreviewDialog()
{
    delete ui;
}

void LoadPreviewDialog::showMyself(QString content)
{
	this->clearMyself();

	host_config_->import_config_content("", content);
	ui->itemListTableWidget->setColumnCount(1);
	ui->itemListTableWidget->setRowCount(host_config_->section_list_.count());
	HostConfig::SectionListIter iter;
	int i = 0;
	for (iter = host_config_->section_list_.begin(); iter != host_config_->section_list_.end(); iter++) {
		QTableWidgetItem *column = new QTableWidgetItem(iter->name_);
		column->setCheckState(Qt::Unchecked);
		if (iter == host_config_->section_list_.begin()) {
			column->setBackgroundColor(QColor(200, 200, 200));
			//column->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		}
		ui->itemListTableWidget->setItem(i, 0, column);
		i++;
	}
	this->show();
	ui->itemListTableWidget->setCurrentItem(ui->itemListTableWidget->item(0, 0));
}

void LoadPreviewDialog::clearMyself()
{
	ui->itemListTableWidget->clear();
	host_config_->section_list_.clear();
}

void LoadPreviewDialog::on_itemListTableWidget_itemSelectionChanged() {
	int current_row = ui->itemListTableWidget->currentRow();
	if (current_row < 0) {
		return;
	}
	int row_count = ui->itemListTableWidget->rowCount();
	int i;
	for (i = 0; i < row_count; i++) {
		QTableWidgetItem *item = ui->itemListTableWidget->item(i, 0);
		QFont font = item->font();
		font.setBold(false);
		item->setFont(font);
	}
	QTableWidgetItem *cur_item = ui->itemListTableWidget->item(current_row, 0);
	QFont font = cur_item->font();
	font.setBold(true);
	cur_item->setFont(font);

	QTextDocument *current_doc = ui->contentEditor->document();
	if (current_doc) {
		current_doc->setPlainText(host_config_->section_list_[current_row].content_);
	}
	ui->contentEditor->setReadOnly(true);
}

void LoadPreviewDialog::on_itemListTableWidget_itemChanged(QTableWidgetItem *item) {
	int row = item->row();
	bool is_enable;
	HostConfig::Section &cur_section = host_config_->section_list_[row];
	if (item->checkState() == Qt::Unchecked) {
		is_enable = false;
	} else {
		is_enable = true;
	}
	cur_section.is_enable_ = is_enable;
	cur_section.name_ = item->text();
}

void LoadPreviewDialog::saveConfig()
{
	HostConfig::SectionListIter iter;
	int i = 0;
	for (iter = host_config_->section_list_.begin(); iter != host_config_->section_list_.end(); iter++) {
		QTableWidgetItem *item = ui->itemListTableWidget->item(i, 0);
		if (item->checkState() == Qt::Checked) {
			if (i == 0) {
				this->parent->host_config_->merge(item->text(), iter->content_);
			} else {
				this->parent->host_config_->set(item->text(), iter->content_);
			}
		}
		i++;
	}
	parent->host_config_->save_info();
	parent->resetItems();
	parent->selectItem(0);
	this->close();
}

void LoadPreviewDialog::on_selectAllBox_stateChanged()
{
	Qt::CheckState state = ui->selectAllBox->checkState();
	HostConfig::SectionListIter iter;
	int i = 0;
	for (iter = host_config_->section_list_.begin(); iter != host_config_->section_list_.end(); iter++) {
		QTableWidgetItem *item = ui->itemListTableWidget->item(i, 0);
		item->setCheckState(state);
		i++;
	}
}
