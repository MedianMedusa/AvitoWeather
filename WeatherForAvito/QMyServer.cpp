#include "QMyServer.h"

QMyServer::QMyServer(QObject *parent)
	: QTcpServer(parent)
{
}

QMyServer::~QMyServer()
{
}
