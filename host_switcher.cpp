#include "host_switcher.h"
#include <QDesktopWidget>
#include <QtGui>
#include <iostream>
#include <QtDebug>

HostSwitcher::HostSwitcher(QWidget *parent) :
	QWidget(parent) {

	ui.setupUi(this);

	Qt::WindowFlags flags = 0;
	flags |= Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint;
	setWindowFlags(flags); // forbid maximum the window

	ui.saveInfoButton->setEnabled(false);

	host_config_ = new HostConfig();

	createTrayIcon();

	ui.itemListTableWidget->setColumnCount(1);
	ui.itemListTableWidget->setRowCount(host_config_->section_list_.count());
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
		ui.itemListTableWidget->setItem(i, 0, column);
		i++;
	}

	QDesktopWidget* desktop = QApplication::desktop();
	move((desktop->width() - this->width()) / 2,	(desktop->height() - this->height()) / 2);

	trayIcon->show();

	load_config_dialog_ = new LoadConfigDialog(this);
	connect(ui.loadConfigButton, SIGNAL(clicked()), this, SLOT(showLoadConfigDialog()));

	setHotKeyDialog = new SetHotkeyDialog(this);
	connect(ui.hotKeyButton, SIGNAL(clicked()), this, SLOT(showSetHotkeyDialog()));

	ui.itemListTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.itemListTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.itemListTableWidget->setFocus();

	this->bindHotkeys();

	this->selectItem(0);
}

HostSwitcher::~HostSwitcher() {
	delete host_config_;
}

void HostSwitcher::selectItem(int i)
{
	this->ui.itemListTableWidget->setCurrentItem(this->ui.itemListTableWidget->item(i, 0));
}

void HostSwitcher::bindHotkeys()
{
#ifdef Q_OS_MAC
	scSwitchDown = new QxtGlobalShortcut(QKeySequence(this->host_config_->config_[HostConfig::HOTKEY_MOVE_DOWN_KEY]), this);
	if (!connect(scSwitchDown, SIGNAL(activated()),this, SLOT(switchItemDown()))) {
		delete scSwitchDown;
		scSwitchDown = NULL;
	}

	scSwitchUp = new QxtGlobalShortcut(QKeySequence(this->host_config_->config_[HostConfig::HOTKEY_MOVE_UP_KEY]), this);
	if (!connect(scSwitchUp, SIGNAL(activated()),this, SLOT(switchItemUp()))) {
		delete scSwitchUp;
		scSwitchUp = NULL;
	}
	scRestore = NULL;
#else
	scRestore = new QxtGlobalShortcut(QKeySequence(this->host_config_->config_[HostConfig::HOTKEY_RESTORE_KEY]), this);
	if (!connect(scRestore, SIGNAL(activated()),this, SLOT(showNormal()))) {
		delete scRestore;
		scRestore = NULL;
	}

	scSwitchDown = new QxtGlobalShortcut(QKeySequence(this->host_config_->config_[HostConfig::HOTKEY_MOVE_DOWN_KEY]), this);
	if (!connect(scSwitchDown, SIGNAL(activated()),this, SLOT(switchItemDown()))) {
		delete scSwitchDown;
		scSwitchDown = NULL;
	}

	scSwitchUp = new QxtGlobalShortcut(QKeySequence(this->host_config_->config_[HostConfig::HOTKEY_MOVE_UP_KEY]), this);
	if (!connect(scSwitchUp, SIGNAL(activated()),this, SLOT(switchItemUp()))) {
		delete scSwitchUp;
		scSwitchUp = NULL;
	}
#endif
}

void HostSwitcher::resetItems() {
	ui.itemListTableWidget->setColumnCount(1);
	ui.itemListTableWidget->setRowCount(host_config_->section_list_.count());
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
		ui.itemListTableWidget->setItem(i, 0, column);
		i++;
	}
	this->resetTrayIconMenu();
}

void HostSwitcher::on_addItemButton_clicked() {
	int last_row = host_config_->section_list_.count();
	host_config_->append_item("new item", "");
	QTableWidgetItem *column = new QTableWidgetItem("new item");
	column->setCheckState(Qt::Unchecked);
	ui.itemListTableWidget->insertRow(last_row);
	ui.itemListTableWidget->setItem(last_row, 0, column);
	ui.itemListTableWidget->editItem(column);
	resetTrayIconMenu();
}

void HostSwitcher::on_deleteItemButton_clicked() {
	int current_row = ui.itemListTableWidget->currentRow();
	if (current_row > 0) {
		ui.itemListTableWidget->removeRow(current_row);
		host_config_->delete_item(current_row);
		host_config_->save_info();
		resetTrayIconMenu();
	}
}

void HostSwitcher::on_moveItemUpButton_clicked() {
	int current_row = ui.itemListTableWidget->currentRow();
	int next_row;
	if (current_row <= 1) {
		next_row = 1;
	} else {
		next_row = current_row - 1;
	}
	this->swapItem(current_row, next_row);
	QTableWidgetItem *next_item = ui.itemListTableWidget->item(next_row, 0);
	ui.itemListTableWidget->setCurrentItem(next_item);
}

void HostSwitcher::on_moveItemDownButton_clicked() {
	int current_row = ui.itemListTableWidget->currentRow();
	int row_count = ui.itemListTableWidget->rowCount();
	int next_row;
	if (current_row >= row_count - 1) {
		next_row = current_row;
	} else {
		next_row = current_row + 1;
	}
	this->swapItem(current_row, next_row);
	QTableWidgetItem *next_item = ui.itemListTableWidget->item(next_row, 0);
	ui.itemListTableWidget->setCurrentItem(next_item);
}

void HostSwitcher::swapItem(int rowA, int rowB) {
	host_config_->section_list_.swap(rowA, rowB);
	host_config_->save_info();
	QTableWidgetItem *itemA = ui.itemListTableWidget->item(rowA, 0);
	QTableWidgetItem *itemB = ui.itemListTableWidget->item(rowB, 0);
	QString title = itemA->text();
	itemA->setText(itemB->text());
	itemB->setText(title);
	Qt::CheckState check_state = itemA->checkState();
	itemA->setCheckState(itemB->checkState());
	itemB->setCheckState(check_state);
}

void HostSwitcher::on_saveInfoButton_clicked() {
	int current_row = ui.itemListTableWidget->currentRow();
	host_config_->section_list_[current_row].content_
			= ui.contentEditor->toPlainText();
	host_config_->save_info();
	QTextDocument *current_doc = ui.contentEditor->document();
	current_doc->setPlainText(host_config_->section_list_[current_row].content_);
	ui.saveInfoButton->setEnabled(false);
	ui.loadConfigButton->setEnabled(true);
}

void HostSwitcher::on_itemListTableWidget_itemSelectionChanged() {
	int current_row = ui.itemListTableWidget->currentRow();
	if (current_row < 0) {
		return;
	}
	int row_count = ui.itemListTableWidget->rowCount();
	if (current_row == 0) {
		ui.deleteItemButton->setEnabled(false);
		ui.moveItemUpButton->setEnabled(false);
		ui.moveItemDownButton->setEnabled(false);
	} else {
		ui.deleteItemButton->setEnabled(true);
		if (current_row == 1) {
			ui.moveItemUpButton->setEnabled(false);
		} else {
			ui.moveItemUpButton->setEnabled(true);
		}
		if (current_row == row_count - 1) {
			ui.moveItemDownButton->setEnabled(false);
		} else {
			ui.moveItemDownButton->setEnabled(true);
		}
	}
	int i;
	for (i = 0; i < row_count; i++) {
		QTableWidgetItem *item = ui.itemListTableWidget->item(i, 0);
		QFont font = item->font();
		font.setBold(false);
		item->setFont(font);
	}
	QTableWidgetItem *cur_item = ui.itemListTableWidget->item(current_row, 0);
	QFont font = cur_item->font();
	font.setBold(true);
	cur_item->setFont(font);

	QTextDocument *current_doc = ui.contentEditor->document();
	if (current_doc) {
		current_doc->setPlainText(host_config_->section_list_[current_row].content_);
	}
	ui.contentEditor->setReadOnly(false);
}

void HostSwitcher::on_itemListTableWidget_itemChanged(QTableWidgetItem *item) {
	int row = item->row();
	bool is_enable;
	HostConfig::Section &cur_section = host_config_->section_list_[row];
	if (item->checkState() == Qt::Unchecked) {
		is_enable = false;
	} else {
		is_enable = true;
	}
	QList<QAction *> actions = trayIconMenu->actions();
	int action_index = 0;
	if (row == 0) {
		action_index = 0;
	} else {
		action_index = row + 1;
	}
	if (is_enable != cur_section.is_enable_) {
		if (row > 0) {
			int row_count = ui.itemListTableWidget->rowCount();
			int i;
			for (i = 1; i < row_count; i++) {
				QTableWidgetItem *item = ui.itemListTableWidget->item(i, 0);
				if (item) {
					if (host_config_->section_list_[i].is_enable_ && i != row) {
						item->setCheckState(Qt::Unchecked);
					}
				}
			}
		}
		cur_section.is_enable_ = is_enable;
		actions[action_index]->setChecked(is_enable);
	}
	cur_section.name_ = item->text();
	actions[action_index]->setText(item->text());
	host_config_->save_info();

}

void HostSwitcher::on_contentEditor_undoAvailable(bool available) {
	ui.saveInfoButton->setEnabled(available);
	ui.loadConfigButton->setEnabled(!available);
}

void HostSwitcher::createTrayIcon()
{
	trayIconMenu = new QMenu(QApplication::desktop());
    connect(trayIconMenu, SIGNAL(triggered(QAction *)), this, SLOT(hostConfigTriggered(QAction *)));
    resetTrayIconMenu();

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    QIcon icon(":/images/switch.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
    trayIcon->setToolTip("Host Switcher");

    trayIcon->setVisible(true);
}

void HostSwitcher::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
		hide();
        this->showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

void HostSwitcher::hostConfigTriggered(QAction *action) {
	int type = action->data().toInt();
	switch (type) {
	case -1:
		hide();
		showNormal();
		break;
	case -2:
		qApp->quit();
		break;
	default:
		QTableWidgetItem *item = ui.itemListTableWidget->item(type, 0);
		if (action->isChecked()) {
			item->setCheckState(Qt::Checked);
		} else {
			item->setCheckState(Qt::Unchecked);
		}
	}
}

void HostSwitcher::resetTrayIconMenu() {
	trayIconMenu->clear();

	HostConfig::SectionListIter iter;
	int i = 0;
	for (iter = host_config_->section_list_.begin(); iter != host_config_->section_list_.end(); iter++) {
		QAction *action = new QAction(iter->name_, this);
		action->setData(QVariant(i));
		action->setCheckable(true);
		action->setChecked(iter->is_enable_);
	    trayIconMenu->addAction(action);
	    if (i == 0) {
	        trayIconMenu->addSeparator();
	    }
		i++;
	}

    restoreAction = new QAction(tr("&Restore"), this);
    restoreAction->setData(QVariant(-1));

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setData(QVariant(-2));

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
}

void HostSwitcher::switchItemDown() {
	int len = host_config_->section_list_.size();
	if (len <= 1) {
		return;
	}
	int i, next = 1;
	for (i = 1; i < len; i++) {
		QTableWidgetItem *item = ui.itemListTableWidget->item(i, 0);
		if (item->checkState() == Qt::Checked) {
			item->setCheckState(Qt::Unchecked);
			next = i % (len - 1) + 1;
			break;
		}
	}
	QTableWidgetItem *item = ui.itemListTableWidget->item(next, 0);
	item->setCheckState(Qt::Checked);
	trayIcon->showMessage("Host Switcher", host_config_->section_list_[next].name_, QSystemTrayIcon::NoIcon, 3000);
}

void HostSwitcher::switchItemUp() {
	int len = host_config_->section_list_.size();
	if (len <= 1) {
		return;
	}
	int i, next = 1;
	for (i = 1; i < len; i++) {
		QTableWidgetItem *item = ui.itemListTableWidget->item(i, 0);
		if (item->checkState() == Qt::Checked) {
			item->setCheckState(Qt::Unchecked);
			next = (i - 2 + len - 1) % (len - 1) + 1;
			break;
		}
	}
	QTableWidgetItem *item = ui.itemListTableWidget->item(next, 0);
	item->setCheckState(Qt::Checked);
	trayIcon->showMessage("Host Switcher", host_config_->section_list_[next].name_, QSystemTrayIcon::NoIcon, 3000);
}

void HostSwitcher::showLoadConfigDialog() {
	load_config_dialog_->show_myself();
}

void HostSwitcher::showSetHotkeyDialog() {
	setHotKeyDialog->showMyself();
}
