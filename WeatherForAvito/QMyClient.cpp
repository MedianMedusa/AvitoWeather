#include "QMyClient.h"


QMyClient::QMyClient(QObject *parent)
	: QNetworkAccessManager(parent)
{
	connect(this, SIGNAL(finalUrlReady()), SLOT(onFinalUrlReady()));
}

QMyClient::~QMyClient()
{
}

void QMyClient::checkConfig()
{
	QFile configuration("Weather.conf");

	if (configuration.open(QIODevice::ReadOnly))
	{
		apiUrl.setUrl(configuration.readLine().trimmed());
		qDebug() << "The API URL is set to " << apiUrl.toString() << endl;
		emit configSuccess();
	}
	else
	{
		qDebug() << "no configuration file found";
		//emit configFail();
		std::exit(1);
	}
}

void QMyClient::setRequest(QString req)
{
	userRequest = req;
	makeFinalUrl();
}

void QMyClient::makeFinalUrl()
{
	qDebug() << userRequest;
	finalUrl = apiUrl.toString();
	finalUrl.append("data/2.5/");

	//forecast
	if (userRequest.left(8) == "forecast")
	{
		//check timestamp
		finalUrl.append(QString("forecast?units=metric&APPID=%1&dt=").arg(token));
		if (userRequest.contains("dt"))
		{
			QRegularExpression regExp("dt=(.*?)[^&# ]*");
			finalUrl.append(regExp.match(userRequest).captured() + "&q=");
		}
		else
		{
			qDebug() << "Timestamp undefined";
			emit responseReady("Timestamp undefined");
			return;
		}
	}

	//current
	else if (userRequest.left(7) == "current")
		finalUrl.append(QString("weather?units=metric&APPID=%1&q=").arg(token));
	else
	{
		qDebug() << "Invalid request";
		emit responseReady("Incorrect address");
		return;
	}

	//add city
	if (userRequest.contains("city="))
	{
		QRegularExpression regExp("city=(.*?)[^&# ]*");
		QString match = regExp.match(userRequest).captured();
		finalUrl.append(match.right(match.length() - 5) + "&");
	}

	else
	{
		qDebug() << "City undefined";
		emit responseReady("City undefined");
		return;
	}
	emit finalUrlReady();
}

void QMyClient::onFinalUrlReady()
{
	qDebug() <<" Connecting to "<< finalUrl;
	connect(this, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
	get(QNetworkRequest(QUrl(finalUrl)));
}

void QMyClient::replyFinished(QNetworkReply* reply)
{
	QString serverResponse = reply->readAll();
	serverResponse = parseJson(serverResponse);
	emit responseReady(serverResponse);
}

QString QMyClient::parseJson(QString s)
{
	QJsonDocument json = QJsonDocument::fromJson(s.toUtf8());
	if (!json.isNull())
	{
		QJsonObject obj = json.object();
		QJsonObject responseJson;
		responseJson.insert("city", obj.value("name"));
		responseJson.insert("unit", "celsius");
		responseJson.insert("temperature", obj.value("main").toObject().value("temp"));
		QJsonDocument responseJsonDocument;
		responseJsonDocument.setObject(responseJson);

		return responseJsonDocument.toJson();
	}
	else 
		return "Invalid response from server";
}