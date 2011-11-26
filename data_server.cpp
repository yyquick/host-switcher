#include "data_server.h"

DataServer::DataServer(QObject *parent) :
    QTcpServer(parent)
{
}

void DataServer::on_newConnection()
{

}
