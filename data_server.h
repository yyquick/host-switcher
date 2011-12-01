#ifndef DATA_SERVER_H
#define DATA_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "host_switcher.h"

#define HS_DATA_SERVER_PORT 12311

class DataServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit DataServer(QObject *parent = 0);
	void incomingConnection(int socket);

	void pause()
	{
		disabled = true;
	}

	void resume()
	{
		disabled = false;
	}

private slots:
	void readClient();
	void discardClient();

private:
	bool disabled;
	HostSwitcher *parent;
};

#endif // DATA_SERVER_H
