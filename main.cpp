#include "mainviewer.h"
#include "MaximumWallThickness.h"

#include <QtWidgets/QApplication>
#include <qfileinfo.h>

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
int main(int argc, char *argv[])
{
	//if (argc == 2) {

		QApplication a(argc, argv);
		MainViewer w;

		if(w.setFileName(QFileInfo("C:/Users/jieji/Desktop/MACOSX_BUNDLE/JackyData/nifti_corrected/segmentation_right.nii").absoluteFilePath())){
		//if (w.setFileName(argv[1])) {
			w.show();
			MaximumWallThickness m(w.loadNIFTIFile());
			int extent[6] = { 128,181,214,286,1,54 };
			m.setExtent(extent);
			m.valueTransform();
			m.extractVOI();
			m.thresholdImage();
			m.edgeDetection();
			m.thicknessCal();
			m.output();


			w.visualizeImage(m.image);

			return a.exec();
		}
	//}

	return 0;
}
