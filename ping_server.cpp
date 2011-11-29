#include "ping_server.h"
#include <QHostInfo>

PingServer::PingServer(QObject *parent) :
    QObject(parent)
{
}

void PingServer::initServer()
{
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(QHostAddress::Any, 12312);

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
	if (type == PingServer::Ping) {
		char sendType = PingServer::Info;
		QString hostName = QHostInfo::localHostName();
		QByteArray data;
		data.append(&sendType, 1);
		data.append(hostName);
		udpSocket->writeDatagram(data, sender, 12312);
	}
	if (type == PingServer::Info) {

	}
}
