#ifndef DATA_SERVER_H
#define DATA_SERVER_H

#include <QTcpServer>
#include "host_switcher.h"

class DataServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit DataServer(QObject *parent = 0);

protected:
	HostSwitcher *parent;

signals:

public slots:
	void on_newConnection();

};

#endif // DATA_SERVER_H
