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
	this->reply_ = NULL;
	connect(this->ui->loadButton, SIGNAL(clicked()), this, SLOT(start_request()));
	connect(this->ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(start_request()));
	connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	ui->cancelButton->setDefault(false);
	ui->loadButton->setDefault(true);
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
		parent_->host_config_->config_["last_load_url"] = url_.toString();
		parent_->loadPreviewDialog->showMyself(this->result_);
		this->ui->statusLabel->setText("");
		this->close();
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
	QString url = parent_->host_config_->config_["last_load_url"];
	if (url != "") {
		ui->lineEdit->setText(url);
	} else {
		ui->lineEdit->setText("http://host/path/file");
	}
	ui->lineEdit->selectAll();
	ui->lineEdit->setFocus();
	show();
}

void LoadConfigDialog::cancel() {
	if (reply_) {
		reply_->abort();
	}
	reject();
}
