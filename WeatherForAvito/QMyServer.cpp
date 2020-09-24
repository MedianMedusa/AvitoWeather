#include "QMyServer.h"

QMyServer::QMyServer(QObject *parent)
	: QTcpServer(parent)
{
	client = new QMyClient();
	connect(client, SIGNAL(configSuccess()), this, SLOT(onClientConfigSuccess()));
	connect(client, SIGNAL(responseReady(QString)), this, SLOT(sendResponse(QString)));
	client->checkConfig();

	connect(this, SIGNAL(responseReady(QString)), this, SLOT(sendResponse(QString)));

	connect(&socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(&socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

QMyServer::~QMyServer()
{
}

void QMyServer::incomingConnection(qintptr handle)
{
	//socket = new QTcpSocket;
	currentDescriptor = handle;
	socket.setSocketDescriptor(handle);
}

void QMyServer::onReadyRead()
{
	qDebug() << "Connected";
	QString request;
	request = socket.readLine();
	handleUserRequest(request);
}

void QMyServer::onClientConfigSuccess()
{
	if (listen(QHostAddress::Any, 8000))
		qDebug() << "Listening...";
	else
	{
		qDebug() << "Error while starting server: " << errorString();
		std::exit(1);
	}
}

void QMyServer::handleUserRequest(QString req)
{
	qDebug() << "Got request: " << req;
	QStringList requestList = req.split("/");
	if (requestList.size() > 1)
		if (requestList.at(1) == "v1" && requestList.size() > 4)
		{
			client->setRequest(requestList.at(2) + requestList.at(3));
			return;
		}

	qDebug() << "Got incorrect request: " << req;
	//response = "Incorrect address";
	emit responseReady("Incorrect address");
	return;
}

void QMyServer::sendResponse(QString response)
{
	//if (response.isEmpty())
	//	qDebug() << "\nAttempt to send an empty response while getdata is: " << client->getData();
	//else
	//	qDebug() << "\nAttempt to send a response: " << response <<"\n\nWhile getdata is: " << client->getData();

	socket.write(QString("HTTP/1.1 200 OK\r\n\r\n%1").arg(response).toLatin1());
	socket.flush();
	socket.disconnectFromHost();
}

void QMyServer::onDisconnected()
{
	//socket.close();
	//socket.deleteLater();
	qDebug() << "Disconnected" << endl;
}

