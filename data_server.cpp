#include <QDateTime>
#include "data_server.h"

DataServer::DataServer(QObject *parent) :
    QTcpServer(parent)
{
	this->parent = (HostSwitcher *)parent;
	listen(QHostAddress::Any, HS_DATA_SERVER_PORT);
	disabled = false;
}

void DataServer::incomingConnection(int socket)
{
	if (disabled)
		return;

	// When a new client connects, the server constructs a QTcpSocket and all
	// communication with the client is done over this QTcpSocket. QTcpSocket
	// works asynchronously, this means that all the communication is done
	// in the two slots readClient() and discardClient().
	QTcpSocket* s = new QTcpSocket(this);
	connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
	connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
	s->setSocketDescriptor(socket);
}

void DataServer::readClient()
{
	if (disabled)
		return;

	// This slot is called when the client sent data to the server. The
	// server looks if it was a get request and sends a very simple HTML
	// document back.
	QTcpSocket* socket = (QTcpSocket*)sender();
	if (socket->canReadLine()) {
		QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
		if (tokens[0] == "GET") {
			QTextStream os(socket);
			os.setAutoDetectUnicode(true);
			os << "HTTP/1.0 200 Ok\r\n"
				"Content-Type: text/plain; charset=\"utf-8\"\r\n"
				"\r\n"
				<< parent->host_config_->getShareData();
			socket->close();

			if (socket->state() == QTcpSocket::UnconnectedState) {
				delete socket;
			}
		}
	}
}

void DataServer::discardClient()
{
	QTcpSocket* socket = (QTcpSocket*)sender();
	socket->deleteLater();
}
