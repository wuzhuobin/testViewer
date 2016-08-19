#include "mainviewer.h"

MainViewer::MainViewer(QWidget *parent)
	: QMainWindow(parent), viewer( vtkSmartPointer<vtkImageViewer2>::New()),
	renderWindow(vtkSmartPointer<vtkRenderWindow>::New())
{
	ui.setupUi(this);

}

MainViewer::~MainViewer()
{

}

bool MainViewer::setFileName(QString fileName)
{
	if (fileName.isEmpty()) return false;
	this->fileName = fileName;

	return true;
}

QString MainViewer::getFileName()
{
	return this->fileName;
}

vtkSmartPointer<vtkImageData> MainViewer::loadNIFTIFile()
{
	vtkSmartPointer<vtkNIFTIImageReader> reader;
	reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	qDebug() << fileName.toLatin1().data();
	reader->SetFileName(fileName.toLatin1().data());
	reader->Update();
	vtkSmartPointer<vtkImageData> img = reader->GetOutput();
	return img;

}


bool MainViewer::visualizeImage(vtkImageData* img)
{
	if(fileName.isEmpty())	return false;
		
	viewer->SetInputData(img);
	double* range = img->GetScalarRange();
	viewer->SetColorWindow(range[1] - range[0]);
	viewer->SetColorLevel((range[1] + range[0]) * 0.5);

	vtkSmartPointer<myVtkInteractorStyleImage> interactorStyle;
	interactorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
	interactorStyle->SetImageViewer(viewer);
	
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	viewer->SetupInteractor(renderWindowInteractor);
	renderWindowInteractor->SetInteractorStyle(interactorStyle);
	//renderWindowInteractor->SetRenderWindow(viewer->GetRenderWindow());

	ui.viewerWidget->SetRenderWindow(viewer->GetRenderWindow());
	viewer->Render();
	viewer->GetRenderer()->ResetCamera();
	viewer->Render();
	//renderWindowInteractor->Start();



	return true;

}

bool MainViewer::visualizePolyData(vtkPolyData * data)
{
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(data);
	mapper->Update();

	vtkSmartPointer<vtkActor> actor = 
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	
	vtkSmartPointer<vtkRenderWindowInteractor> rwinterator =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	rwinterator->SetRenderWindow(renderWindow);

	renderWindow->AddRenderer(renderer);

	ui.viewerWidget->SetRenderWindow(renderWindow);
	renderWindow->Render();
	


	return false;
}
