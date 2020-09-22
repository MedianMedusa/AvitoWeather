#include <QtCore>
#include <iostream>
//#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QUrl>
#include <QJsonDocument>


//QTextStream cout(stdout);
//QTextStream cin(stdin);
using namespace std;

QString token = "05f86f84c1e28be95a4b6b8a0f2eb83a";
QUrl apiUrl;

string toStr(QString str)
{
	return str.toLatin1().constData();
}
bool checkConf()
{
	QFile configuration("Weather.conf");

	if (configuration.open(QIODevice::ReadOnly))
		apiUrl.setUrl(configuration.readLine().trimmed());
	else
	{
		cout << "no configuration file found";
		return true;
	}
	cout << "The API URL is set to " << toStr(apiUrl.toString()) << endl << endl;
	return false;
}



QStringList current(QString req)
{
	apiUrl.setPath("/data/2.5/weather");
	apiUrl.setQuery(QString("q=%1&units=metric&APPID=%2").arg(req).arg(token));


	QStringList result;
	return result;
}

QStringList forecast(QString req)
{
	apiUrl.setPath("/data/2.5/forecast");
	apiUrl.setQuery(QString("q=%1&units=metric&APPID=%2").arg(req).arg(token));

	QStringList result;
	return result;
}



/*******************************************************************************************************************/

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    //return a.exec();
	QTextStream str(stdin);

	if (checkConf()) return 1;

	QString req;
	QStringList result;

	while (req != "q")
	{
		cout << "Write a request (city, \"current\"/timestamp) or \"q\" to quit:" << endl;
		req = str.readLine();
		if (req == "q") return 0;

		QStringList list = req.split(',');
		if (list.size() > 1)
		{
			if (list.at(1).contains("current"))
				result = current(list.at(0));
			else
				result = forecast(list.at(1));
		}
		else cout << "Incorrect request!" << endl;
	}

	for (int i = 0; i < 3; i++)
	{
		switch (i) {
		case 0:
		{
			cout << "\"city\": \"" << toStr(result.at(0)) << "\"," << endl;
			break;
		}
		case 1:
		{
			cout << "\"unit\": \"" << toStr(result.at(1)) << "\"," << endl;
			break;
		}
		case 2:
		{
			cout << "\"temperature\": " << toStr(result.at(2)) << endl;
			break;
		}
		}
	}
	return 0;
}