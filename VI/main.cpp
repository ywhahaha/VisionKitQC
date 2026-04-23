#include "VI.h"
#include <QtWidgets/QApplication>
#include "CCreateDump.h"
int main(int argc, char *argv[])
{
	CCreateDump::Instance()->DeclarDumpFile("dumpfile");
    QApplication a(argc, argv);
    VI w;
    w.show();
    return a.exec();
}
