#pragma once

#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QCoreApplication>
#include <QRegularExpression>

class QMyClient : public QNetworkAccessManager
{
	Q_OBJECT

public:
	QMyClient(QObject *parent = 0);
	~QMyClient();

	void setRequest(QString);
	QString getData();
	void checkConfig();

private:
	QString userRequest;
	QString serverResponse;

	QNetworkRequest* networkRequest;
	QUrl apiUrl;

	QUrl makeFinalUrl();

	const QString token = "05f86f84c1e28be95a4b6b8a0f2eb83a";

signals:
	void configSuccess();
};
