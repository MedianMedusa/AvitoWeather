#pragma once
#include <QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include "QMyClient.h"
#include <QtNetwork/QNetworkReply>

class QMyServer : public QTcpServer
{
	Q_OBJECT

public:
	QMyServer(QObject *parent = 0);
	~QMyServer();
	void incomingConnection(qintptr);

public slots:
	void onReadyRead();
	void onDisconnected();
	void onClientConfigSuccess();
	void handleUserRequest(QString);

private:
	QMyClient *client;
	QString response;
	QTcpSocket *socket;



private slots:
	void replyFinished(QNetworkReply*);
	void sendResponse(QTcpSocket*);

signals:
	void readyToResponse(QTcpSocket*);
};
