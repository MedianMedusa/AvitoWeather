#include "QMyServer.h"

QMyServer::QMyServer(QObject *parent)
	: QTcpServer(parent)
{
	client = new QMyClient();
	connect(client, SIGNAL(configSuccess()), this, SLOT(onClientConfigSuccess()));
	client->checkConfig();
	connect(this, SIGNAL(readyToResponse(QTcpSocket*)), this, SLOT(sendResponse(QTcpSocket*)));
}

QMyServer::~QMyServer()
{
}

void QMyServer::incomingConnection(qintptr handle)
{
	QTcpSocket* socket = new QTcpSocket();
	socket->setSocketDescriptor(handle);

	connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}


void QMyServer::onReadyRead()
{
	qDebug() << "Connected";
	QTcpSocket* socket = qobject_cast <QTcpSocket*> (sender());
	
	QString request = socket->readLine();
	handleUserRequest(request);

}

void QMyServer::onDisconnected()
{
	QTcpSocket* socket = qobject_cast <QTcpSocket*> (sender());
	socket->close();
	socket->deleteLater();
	qDebug() << endl << "disconnected" << endl;
}

void QMyServer::onClientConfigSuccess()
{
	if (listen(QHostAddress::Any, 8000))
		qDebug() << "Listening...";
	else
		qDebug() << "Error while starting: " << errorString();
}

void QMyServer::handleUserRequest(QString req)
{
	qDebug() << "Got request: " << req;
	QStringList requestList = req.split("/");
	if (requestList.size() > 1)
	{
		if (requestList.at(1) == "v1" && requestList.size()>4)
			client->setRequest(requestList.at(2)+requestList.at(3));
		else
		{
			qDebug() << "Got incorrect request: " << req;
			response = "Incorrect address";
		}
	}
	//QUrl asd = "http://api.openweathermap.org/data/2.5/weather?q=Moscow&units=metric&APPID=05f86f84c1e28be95a4b6b8a0f2eb83a";
	//QNetworkAccessManager* manager = new QNetworkAccessManager();
//	QNetworkReply *reply = manager->get(QNetworkRequest(asd));
	//connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	//reply->open(QIODevice::ReadOnly);
	//qDebug() << reply->readAll();
//	qDebug() << manager.get(QNetworkRequest(QUrl("http://api.openweathermap.org/data/2.5/weather?q=Moscow&units=metric&APPID=05f86f84c1e28be95a4b6b8a0f2eb83a")));
	emit readyToResponse();
}

void QMyServer::sendResponse(QTcpSocket* socket)
{
	socket->write(QString("HTTP/1.1 200 OK\r\n\r\n%1").arg(response).toLatin1());
	socket->disconnectFromHost();
}

void QMyServer::replyFinished(QNetworkReply* reply)
{
	qDebug() << reply->readAll();
}