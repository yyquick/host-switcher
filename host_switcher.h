#ifndef HOSTMANAGER_H
#define HOSTMANAGER_H

#include <QtGui/QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ui_host_switcher.h"
#include "host_config.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPushButton;
QT_END_NAMESPACE

class HostSwitcher: public QWidget {
Q_OBJECT

public:
	HostSwitcher(QWidget *parent = 0);
	~HostSwitcher();

	static QString help_message;

private:
	Ui::HostSwitcherClass ui;

	HostConfig *host_config_;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

	void createTrayIcon();
	void resetTrayIconMenu();

public slots:
	void on_addItemButton_clicked();
	void on_deleteItemButton_clicked();

	void on_saveInfoButton_clicked();

	void on_itemListTableWidget_itemSelectionChanged();
	void on_itemListTableWidget_itemChanged(QTableWidgetItem *item);

	void on_contentEditor_undoAvailable(bool available);

	void iconActivated(QSystemTrayIcon::ActivationReason reason);

	void hostConfigTriggered(QAction *action);

	void switchItemUp();
	void switchItemDown();
};

#endif // HOSTMANAGER_H
