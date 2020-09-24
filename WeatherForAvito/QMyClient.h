#pragma once

#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class QMyClient : public QNetworkAccessManager
{
	Q_OBJECT

public:
	QMyClient(QObject *parent = 0);
	~QMyClient();

	void checkConfig();
	void setRequest(QString);

private:
	const QString token = "05f86f84c1e28be95a4b6b8a0f2eb83a";
	QString userRequest;

	QUrl apiUrl;
	QString finalUrl;
	void makeFinalUrl();

	QString parseJson(QString);

private slots:
	void onFinalUrlReady();
	void replyFinished(QNetworkReply*);

signals:
	void configSuccess();
	void finalUrlReady();
	void responseReady(QString);
};
