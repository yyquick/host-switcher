#ifndef PING_SERVER_H
#define PING_SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>

class PingServer : public QObject
{
    Q_OBJECT
public:
	enum {
		Ping,
		Info
	};

    explicit PingServer(QObject *parent = 0);

	void initServer();

signals:

public slots:
	void readPendingDatagrams();
	void processTheDatagram(QByteArray datagram, QHostAddress &sender);

protected:
	QUdpSocket *udpSocket;
};

#endif // PING_SERVER_H
