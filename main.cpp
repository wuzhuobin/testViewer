#include "mainviewer.h"
#include "MaximumWallThickness.h"

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
			MaximumWallThickness m(w.loadNIFTIFile());
			int extent[6] = {255,511,127,383,22,50};
			m.setExtent(extent);
			m.valueTransform();
			m.extractVOI();
			m.thresholdImage();
			m.edgeDetection();
			m.thicknessCal();
			m.output();

			w.visualizeImage(m.edgeImage[1]);

			return a.exec();
		}
	}

	return 0;
}
