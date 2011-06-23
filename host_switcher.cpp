#include "host_switcher.h"
#include <QDesktopWidget>
#include <QtGui>
#include <iostream>
#include <QtDebug>
#include <qxtglobalshortcut.h>

#ifdef Q_OS_MAC
QString HostSwitcher::help_message = "Welcome to use HostSwitcher(version 0.2)!\n"
		"\n"
				"Press \"Command+Shift+S\" or \"Command+Shift+W\" to switch the host config.(new feature)\n"
		"\n"
		"Clicking the items in the left side can edit the host config and enable/disable it.\n"
		"\n"
		"The 'Common' item is used for the common host config. You can enable/disable it individually. And you can enable only one of the other items.\n"
		"\n"
		"You can edit the config content of the selected item on the editor on the right side and save the change by clicking the 'Save' button.\n"
		"\n"
		"You can add or delete items by clicking the relative buttons on the leftdown side.\n"
		"\n"
		"And you can alse switch the host config by right-clicking the system tray icon.\n"
		"\n"
		"You can not close the program by closing the window. You should close it through the system tray icon.\n"
		"\n"
		"This software is written by yyquick. If you can give him a delicious chicken leg, he will smile :-)";
#else
QString HostSwitcher::help_message = "Welcome to use HostSwitcher(version 0.2)!\n"
                "\n"
				"Press \"Ctrl+Alt+H\" to restore this window.(new feature)\n"
                "\n"
				"Press \"Ctrl+Alt+S\" or \"Ctrl+Alt+W\" to switch the host config.(new feature)\n"
                "\n"
                "Clicking the items in the left side can edit the host config and enable/disable it.\n"
                "\n"
                "The 'Common' item is used for the common host config. You can enable/disable it individually. And you can enable only one of the other items.\n"
                "\n"
                "You can edit the config content of the selected item on the editor on the right side and save the change by clicking the 'Save' button.\n"
                "\n"
                "You can add or delete items by clicking the relative buttons on the leftdown side.\n"
                "\n"
                "And you can alse switch the host config by right-clicking the system tray icon.\n"
                "\n"
                "You can not close the program by closing the window. You should close it through the system tray icon.\n"
                "\n"
                "This software is written by yyquick. If you can give him a delicious chicken leg, he will smile :-)";
#endif

HostSwitcher::HostSwitcher(QWidget *parent) :
	QWidget(parent) {

	ui.setupUi(this);
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

	ui.contentEditor->setPlainText(help_message);
	ui.contentEditor->setReadOnly(true);

#ifdef Q_OS_MAC
	QxtGlobalShortcut * scSwitchDown = new QxtGlobalShortcut(QKeySequence("Ctrl+Shift+S"), this);
	connect(scSwitchDown, SIGNAL(activated()),this, SLOT(switchItemDown()));

	QxtGlobalShortcut * scSwitchUp = new QxtGlobalShortcut(QKeySequence("Ctrl+Shift+W"), this);
	connect(scSwitchUp, SIGNAL(activated()),this, SLOT(switchItemUp()));
#else
	QxtGlobalShortcut * scRestore = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+H"), this);
	connect(scRestore, SIGNAL(activated()),this, SLOT(showNormal()));

	QxtGlobalShortcut * scSwitchDown = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+S"), this);
	connect(scSwitchDown, SIGNAL(activated()),this, SLOT(switchItemDown()));

	QxtGlobalShortcut * scSwitchUp = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+W"), this);
	connect(scSwitchUp, SIGNAL(activated()),this, SLOT(switchItemUp()));
#endif

	ui.itemListTableWidget->setFocus();
}

HostSwitcher::~HostSwitcher() {
	delete host_config_;
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

void HostSwitcher::on_saveInfoButton_clicked() {
	int current_row = ui.itemListTableWidget->currentRow();
	host_config_->section_list_[current_row].content_
			= ui.contentEditor->toPlainText();
	host_config_->save_info();
	QTextDocument *current_doc = ui.contentEditor->document();
	current_doc->setPlainText(host_config_->section_list_[current_row].content_);
	ui.saveInfoButton->setEnabled(false);
}

void HostSwitcher::on_itemListTableWidget_itemSelectionChanged() {
	int current_row = ui.itemListTableWidget->currentRow();
	if (current_row < 0) {
		return;
	}
	if (current_row == 0) {
		ui.deleteItemButton->setEnabled(false);
	} else {
		ui.deleteItemButton->setEnabled(true);
	}
	int i;
	int row_count = ui.itemListTableWidget->rowCount();
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
		current_doc->setPlainText(
				host_config_->section_list_[current_row].content_);
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
}

void HostSwitcher::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
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
