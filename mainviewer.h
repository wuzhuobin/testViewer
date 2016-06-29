#ifndef MAINVIEWER_H
#define MAINVIEWER_H

#include <QtWidgets/QMainWindow>
#include <QString>
#include <QDebug>

#include <vtkImageViewer2.h>
#include <vtkSmartPointer.h>
#include <vtkNIFTIImageReader.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>

#include <itkImageFileReader.h>
#include <itkSmartPointer.h>
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>

#include "vtkProcessor.h"
#include "myVtkInteractorStyleImage.h"
#include "ui_mainviewer.h"

class MainViewer : public QMainWindow
{
	Q_OBJECT

public:
	MainViewer(QWidget *parent = 0);
	~MainViewer();

	bool setFileName(QString fileName);
	QString getFileName();
	vtkSmartPointer<vtkImageData> loadFile();
	bool visualizeImage(vtkImageData* img);
	bool visualizePolyData(vtkPolyData* data);

private:
	
	QString fileName;

	vtkSmartPointer<vtkImageViewer2> viewer;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	Ui::MainViewerClass ui;
};

#endif // MAINVIEWER_H
