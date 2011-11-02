#include "load_config_dialog.h"
#include "ui_load_config_dialog.h"
#include <QtNetwork>
#include <QtDebug>

LoadConfigDialog::LoadConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadConfigDialog)
{
    ui->setupUi(this);
	this->parent_ = (HostSwitcher *)parent;
	connect(this->ui->loadButton, SIGNAL(clicked()), this, SLOT(start_request()));
	connect(this->ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(start_request()));
	connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
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
	url_.setUrl(this->ui->lineEdit->text());
	reply_ = qnam_.get(QNetworkRequest(url_));
	connect(reply_, SIGNAL(finished()),	this, SLOT(http_finished()));
	connect(reply_, SIGNAL(readyRead()), this, SLOT(http_ready_read()));
	this->ui->statusLabel->setText("Loading...");
}

void LoadConfigDialog::http_finished()
{
	if (this->reply_->error() == QNetworkReply::NoError) {
		parent_->host_config_->import_config_content(url_.toString(), this->result_);
		this->ui->statusLabel->setText("");
		this->close();
		parent_->showHelpMessage();
		parent_->resetItems();
	} else {
		this->ui->statusLabel->setText(this->reply_->errorString());
		this->ui->statusLabel->setToolTip(this->reply_->errorString());
	}
}

void LoadConfigDialog::http_ready_read()
{
	if (reply_->error() == QNetworkReply::NoError) {
		this->result_ = reply_->readAll();
	}
}

void LoadConfigDialog::show_myself() {
	if (parent_->host_config_->last_load_url_ != "") {
		ui->lineEdit->setText(parent_->host_config_->last_load_url_);
	} else {
		ui->lineEdit->setText("http://host/path/file");
	}
	ui->lineEdit->selectAll();
	ui->lineEdit->setFocus();
	show();
}

void LoadConfigDialog::cancel() {
	reply_->abort();
	reject();
}
