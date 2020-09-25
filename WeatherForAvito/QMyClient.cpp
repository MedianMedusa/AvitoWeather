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
		qDebug() << "The API URL is set to " << apiUrl.toString();
		emit configSuccess();
	}
	else
	{
		qDebug() << "No configuration file found";
		//emit configFail();
		std::exit(1); //toDo: переделать систему аварийного выхода
	}
}

void QMyClient::setRequest(QString req)
{
	userRequest = req;
	makeFinalUrl();
}

void QMyClient::makeFinalUrl()
{
	finalUrl = apiUrl.toString();
	finalUrl.append("data/2.5/");

	//forecast
	if (userRequest.left(8) == "forecast")
	{
		
		finalUrl.append(QString("forecast?units=metric&APPID=%1&q=").arg(token));

		//check timestamp
		if (userRequest.contains("dt="))
		{
			//set accessible timestamp (nearest minimal)
			if ((timeStamp = abs(QRegularExpression("dt=(.*?)[^&# ]*").match(userRequest).captured().mid(3).toInt())) > 0)
			{
				if(timeStamp > QDateTime::currentDateTime().toTime_t())
					if (timeStamp < QDateTime::currentDateTime().toTime_t()+421200)
						timeStamp -= timeStamp % 10800;
					else
					{
						qDebug() << "Timestamp incorrect";
						emit responseReady("This timestamp is too far away to forecast");
						return;
					}
				else
				{
					qDebug() << "Timestamp incorrect. got: " << timeStamp << " and the current is: " << QDateTime::currentDateTime().toTime_t();
					emit responseReady("This timestamp has already passed");
					return;
				}
			}
			else
			{
				qDebug() << "Timestamp incorrect";
				emit responseReady("Incorrect timestamp");
				return;
			}
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
		QString match = QRegularExpression("city=(.*?)[^&# ]*").match(userRequest).captured();
		finalUrl.append(match.mid(5) + "&");
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
	qDebug() <<"Connecting to "<< finalUrl;
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
		//check response code:

		if (obj.value("cod") != "200" && obj.value("cod") != 200)
		{
			//qDebug() << obj.value("cod");
			responseJson.insert("cod", obj.value("cod"));
			responseJson.insert("message", obj.value("message"));

			QJsonDocument responseJsonDocument;
			responseJsonDocument.setObject(responseJson);
			return responseJsonDocument.toJson();
		}

		//city:
		responseJson.insert("city", obj.value("name"));
		if (responseJson.value("city").isUndefined())
			responseJson.insert("city", obj.value("city").toObject().value("name"));

		//unit:
		responseJson.insert("unit", "celsius");

		//temp:
		responseJson.insert("temperature", obj.value("main").toObject().value("temp"));
		if (responseJson.value("temperature").isUndefined())
			foreach(QJsonValue v, obj.value("list").toArray())
			{
				QJsonObject obj_ = v.toObject();
				if ((uint)obj_.value("dt").toInt() == timeStamp)
				{
					responseJson.insert("temperature", obj_.value("main").toObject().value("temp"));
					break;
				}
			}

		QJsonDocument responseJsonDocument;
		responseJsonDocument.setObject(responseJson);

		return responseJsonDocument.toJson();
	}
	else 
		return "Invalid response from server";
}