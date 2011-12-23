#include "load_config_dialog.h"
#include "ui_load_config_dialog.h"
#include "ping_server.h"
#include <QtNetwork>
#include <QtDebug>
#include <iostream>
#include <QNetworkInterface>
#include <QList>
#include <QHostAddress>
#include <QStringList>
#include <QString>
#include <QAbstractSocket>
#include <QIcon>
#include <iostream>

LoadConfigDialog::LoadConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadConfigDialog)
{
    ui->setupUi(this);
	this->parent_ = (HostSwitcher *)parent;
	this->reply_ = NULL;
	connect(this->ui->loadButton, SIGNAL(clicked()), this, SLOT(start_request()));
	connect(this->ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(start_request()));
	connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	ui->cancelButton->setDefault(false);
	ui->loadButton->setDefault(true);

	QList<QHostAddress> ipList = QNetworkInterface::allAddresses();

	QString myIP, network;
	foreach (QHostAddress addr, ipList) {
		if (addr.toIPv4Address() >> 24 != 127 && addr.protocol() == QAbstractSocket::IPv4Protocol) {
			myIP = addr.toString();
			break;
		}
	}
	QStringList parts = myIP.split('.');
	parts.last() = "*";
	network = parts.join(".");
	ui->ipFilterEdit->setText(network);

	ui->clientsTableWidget->setColumnCount(3);
	ui->clientsTableWidget->setColumnWidth(0, 179);
	ui->clientsTableWidget->setColumnWidth(1, 179);
	ui->clientsTableWidget->setColumnWidth(2, 179);

	lock = false;
}

LoadConfigDialog::~LoadConfigDialog()
{
    delete ui;
}

void LoadConfigDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoadConfigDialog::start_request()
{
	this->result_ = "";
	QString url;
	url = this->ui->lineEdit->text();
	if (!url.startsWith("http://")) {
		url = QString("http://") + url + QString(":") + QString::number(HS_DATA_SERVER_PORT);
	}
	url_.setUrl(url);
	reply_ = qnam_.get(QNetworkRequest(url_));
	connect(reply_, SIGNAL(finished()), this, SLOT(http_finished()));
	connect(reply_, SIGNAL(readyRead()), this, SLOT(http_ready_read()));
	this->ui->statusLabel->setText("Loading...");
}

void LoadConfigDialog::http_finished()
{
	if (this->reply_->error() == QNetworkReply::NoError) {
		QString url = this->ui->lineEdit->text();
		if (url.startsWith("http://")) {
			parent_->host_config_->config_["last_load_url"] = url;
		}
		parent_->loadPreviewDialog->showMyself(this->result_);
		this->ui->statusLabel->setText("");
	} else {
		this->ui->statusLabel->setText(this->reply_->errorString());
		this->ui->statusLabel->setToolTip(this->reply_->errorString());
	}
}

void LoadConfigDialog::http_ready_read()
{
	if (reply_->error() == QNetworkReply::NoError) {
		this->result_ += reply_->readAll();
	}
}

void LoadConfigDialog::show_myself() {
	ui->clientsTableWidget->setCurrentItem(ui->clientsTableWidget->currentItem(), QItemSelectionModel::Clear);
	QString url = parent_->host_config_->config_["last_load_url"];
	if (url != "") {
		ui->lineEdit->setText(url);
	} else {
		ui->lineEdit->setText("http://host/path/file");
	}
	ui->lineEdit->selectAll();
	ui->lineEdit->setFocus();
	on_searchButton_clicked();
	show();
}

void LoadConfigDialog::cancel() {
	if (reply_) {
		reply_->abort();
	}
	hide();
}

void LoadConfigDialog::refreshClients() {
	lock = true;
	ui->clientsTableWidget->setRowCount(parent_->pingServer->clients.count());

	PingServer *pingServer = parent_->pingServer;
	PingServer::ClientIterator iter;
	QString iconPath = QApplication::applicationDirPath() + "/images/computer.gif";
	QMap< QString, QString > reverseMap;
	for (iter = pingServer->clients.begin(); iter != pingServer->clients.end(); iter++) {
		reverseMap.insertMulti(iter.value(), iter.key());
	}
	QMap< QString, QString >::iterator iter2;
	int i = 0;
	for (iter2 = reverseMap.begin(); iter2 != reverseMap.end(); iter2++) {
		QTableWidgetItem *item = new QTableWidgetItem(iter2.key());
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		item->setIcon(QIcon(iconPath));
		item->setToolTip(iter2.value());
		ui->clientsTableWidget->setItem(i / 3, i % 3, item);
		i++;
	}
	lock = false;
}

void LoadConfigDialog::on_searchButton_clicked()
{
	ui->clientsTableWidget->clear();
	QString network = ui->ipFilterEdit->text();
	parent_->pingServer->searchClients(network);
}

void LoadConfigDialog::on_clientsTableWidget_itemSelectionChanged()
{
	if (lock) {
		return;
	}

	QTableWidgetItem *item = ui->clientsTableWidget->currentItem();
	ui->lineEdit->setText(item->toolTip());
}
