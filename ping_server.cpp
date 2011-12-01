#include "ping_server.h"
#include <QHostInfo>
#include <QRegExp>

#include <iostream>

PingServer::PingServer(QObject *parent) :
    QObject(parent)
{
	this->parent = (HostSwitcher *)parent;
}

void PingServer::initServer()
{
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(QHostAddress::Any, HS_PING_SERVER_PORT);

	connect(udpSocket, SIGNAL(readyRead()),
			this, SLOT(readPendingDatagrams()));
}

void PingServer::readPendingDatagrams()
{
	while (udpSocket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;

		udpSocket->readDatagram(datagram.data(), datagram.size(),
								&sender, &senderPort);

		processTheDatagram(datagram, sender);
	}
}

void PingServer::processTheDatagram(QByteArray datagram, QHostAddress &sender)
{
	char type = datagram[0];
	datagram.remove(0, 1);
	short len = *(short *)datagram.data();
	if (len != datagram.length() + 1) {
		return;
	}
	datagram.remove(0, 2);

	if (type == PingServer::Ping) {
		if (parent->host_config_->hasShareData()) {
			char sendType = PingServer::Info;
			QString hostName = QHostInfo::localHostName();
			QByteArray data;
			data.append(&sendType, 1);
			short len;
			len = hostName.length() + 3;
			data.append((char *)&len, 2);
			data.append(hostName);
			udpSocket->writeDatagram(data, sender, HS_PING_SERVER_PORT);
		}
	}
	if (type == PingServer::Info) {
		QString hostName = datagram;
		recordClient(sender, hostName);
	}
}

void PingServer::ping(QHostAddress &receiver)
{
	QByteArray data;
	char sendType = PingServer::Ping;
	data.append(&sendType, 1);
	short len;
	len = 3;
	data.append((char *)&len, 2);
	QUdpSocket usock;
	usock.writeDatagram(data, receiver, HS_PING_SERVER_PORT);
}

void PingServer::recordClient(QHostAddress &addr, QString hostName)
{
	clients[addr.toString()] = hostName;
	parent->load_config_dialog_->refreshClients();
}

void PingServer::searchClients(QString network)
{
	QRegExp rx("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.)\\*");
	if (rx.indexIn(network) != -1) {
		QString ip, prefix;
		QStringList match_list = rx.capturedTexts();
		prefix = match_list[1];
		int i;
		QHostAddress addr;
		for (i = 1; i <= 254; i++) {
			ip = prefix + QString::number(i);
			addr.setAddress(ip);
			ping(addr);
		}
	}
}
