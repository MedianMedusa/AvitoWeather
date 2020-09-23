#include <QtCore>
//#include <iostream>
//#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QUrl>
#include <QJsonDocument>
#include <QDebug>
#include "QMyServer.h"

//QTextStream cout(stdout);
//QTextStream cin(stdin);
//using namespace std;

//QString token = "05f86f84c1e28be95a4b6b8a0f2eb83a";
//QUrl apiUrl;
//QMyClient *client = new QMyClient();

//QStringList current(QString req)
//{
//	apiUrl.setPath("/data/2.5/weather");
//	apiUrl.setQuery(QString("q=%1&units=metric&APPID=%2").arg(req).arg(token));
//
//
//	QStringList result;
//	return result;
//}
//
//QStringList forecast(QString req)
//{
//	apiUrl.setPath("/data/2.5/forecast");
//	apiUrl.setQuery(QString("q=%1&units=metric&APPID=%2").arg(req).arg(token));
//
//	QStringList result;
//	return result;
//}



/*******************************************************************************************************************/

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//QFile configuration("Weather.conf");
	//if (configuration.open(QIODevice::ReadOnly))
	//{
	//	apiUrl.setUrl(configuration.readLine().trimmed());
	//	qDebug() << "The API URL is set to " << apiUrl.toString() << endl;
	//}
	//else
	//{
	//	qDebug() << "no configuration file found";
	//	return 1;
	//}

	QMyServer server;
	//server.setClient(client);
	
	return a.exec();
	
}