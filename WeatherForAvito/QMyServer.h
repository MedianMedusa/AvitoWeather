#pragma once

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>

class QMyServer : public QTcpServer
{
	Q_OBJECT

public:
	QMyServer(QObject *parent);
	~QMyServer();
};
