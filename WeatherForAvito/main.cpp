#include <QCoreApplication>
#include "QMyServer.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QMyServer server;
	
	return a.exec();
	
}