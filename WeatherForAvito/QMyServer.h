#pragma once
#include <QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include "QMyClient.h"
#include <QtNetwork/QNetworkReply>
#include <QPointer>

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
	//QString response;
	//QPointer<QTcpSocket> socket;
	QTcpSocket socket;
	qintptr currentDescriptor;


private slots:
	//void replyFinished(QNetworkReply*);

	void sendResponse(QString);

signals:
	void responseReady(QString);
};
