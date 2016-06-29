#pragma once

#define SEGMENTATION_CLASS 2
#define CLASSI 1
#define CLASSII 0

#include <vtkNIFTIImageReader.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkImageIterator.h>
#include <vtkImageCast.h>
#include <vtkImageSobel2D.h>
#include <vtkImageMathematics.h>
#include <vtkImageExtractComponents.h>
#include <vtkPolyData.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkExtractVOI.h>
#include <vtkImageThreshold.h>

#include <qdebug.h>

#include <math.h>
#include <algorithm>
#include <utility>
#include <list>
#include <fstream>
#include <iostream>

class vtkProcessor
{
public:
	vtkProcessor(std::string fileName);
	~vtkProcessor();

	void process1();
	void process2();
	void process3();
	void process4();
	void process5();
	vtkSmartPointer<vtkImageData> divide(int kind);

	vtkSmartPointer<vtkPolyData> image2Poly(vtkImageData* image);

	vtkImageData* getImage();

//private:

	std::string fileName;
	vtkSmartPointer<vtkImageData> image = 0;
	vtkSmartPointer<vtkImageData> images[SEGMENTATION_CLASS];

	double* centerX[SEGMENTATION_CLASS];
	double* centerY[SEGMENTATION_CLASS];

	// dimension 1 for SEGMENTATION_CLASS, dimension 2 for slices
	std::list<int>* (pointsX[SEGMENTATION_CLASS]);
	std::list<int>* (pointsY[SEGMENTATION_CLASS]);


};

