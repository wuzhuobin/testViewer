#include "mainviewer.h"

#include <QtWidgets/QApplication>

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
int main(int argc, char *argv[])
{
	if (argc == 2) {

		QApplication a(argc, argv);
		MainViewer w;

		if (w.setFileName(argv[1])) {
			w.show();
			//MainViewer w1;
			//w1.show();
			w.setFileName(argv[1]);
			//w1.setFileName(argv[1]);

			return a.exec();
		}
	}

	return 0;
}
