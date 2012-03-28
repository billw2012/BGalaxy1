#include "galaxy1.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Galaxy1 w;
	w.show();
	return a.exec();
}
