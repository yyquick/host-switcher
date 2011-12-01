#ifndef PING_SERVER_H
#define PING_SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>
#include <QMap>
#include <QHostAddress>
#include <QString>
#include "host_switcher.h"

#define HS_PING_SERVER_PORT 12312

class HostSwitcher;

class PingServer : public QObject
{
    Q_OBJECT
public:
	enum {
		Ping,
		Info
	};

	struct ClientInfo {
		QHostAddress addr;
		QString hostname;
	};

	QMap< QString, QString > clients;
	typedef QMap< QString, QString >::iterator ClientIterator;

    explicit PingServer(QObject *parent = 0);
	void initServer();
	void ping(QHostAddress &receiver);
	void searchClients(QString network);

signals:

public slots:
	void readPendingDatagrams();
	void processTheDatagram(QByteArray datagram, QHostAddress &sender);

protected:
	HostSwitcher *parent;
	QUdpSocket *udpSocket;

	void recordClient(QHostAddress &addr, QString hostName);
};

#endif // PING_SERVER_H
