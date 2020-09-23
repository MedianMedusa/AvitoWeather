#include "QMyClient.h"

QMyClient::QMyClient(QObject *parent)
	: QNetworkAccessManager(parent)
{

}

QMyClient::~QMyClient()
{
}

void QMyClient::setRequest(QString req)
{
	userRequest = req;
}

QString QMyClient::getData()
{
	return serverResponse;
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

QUrl QMyClient::makeFinalUrl()
{
	QStringList list = userRequest.split("/");
	QString finalUrl = apiUrl.toString();
	finalUrl.append("data/2.5/");
	if (list.at(0) == "forecast")
		finalUrl.append(QString("forecast?units=metric&APPID=%1&q=").arg(token));
	else if (list.at(0) == "current")
		finalUrl.append(QString("weather?units=metric&APPID=%1&q=").arg(token));

	if (list.at(1).contains("city="))
	{
		QRegularExpressionMatch match;
		finalUrl.append(QString(list.at(1).mid(list.at(1).indexOf("city=") + 5, list.at(1).indexOf("&") - list.at(1).indexOf("city=") - 5)));
		list.at(1).indexOf(QRegularExpression("city=(.*?)[^&#]*"), 0, &match);
		finalUrl.append(match.captured());
	}
	else
	{
		qDebug() << "City undefined";
	}
	//else if (userRequest.co)
	return QUrl();
}
