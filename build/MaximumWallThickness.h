#ifndef MAXIMUMWALLTHICKNESS_H
#define MAXIMUMWALLTHICKNESS_H
#endif // !MAXIUMWALLTHICKNESS_H

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkImageShiftScale.h>
#include <vtkImageTHreshold.h>
#include <vtkExtractVOI.h>
#include <vtkThreshold.h>
#include <vtkImageSobel2D.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageMathematics.h>

#include <utility>
#include <math.h>
#include <list>
#include <iostream>



class MaximumWallThickness
{
public:

	const static int EDGENUM = 2;

	MaximumWallThickness(vtkImageData* image, int internalEdgeValue = 1, int externalEdgeValue = 0);
	~MaximumWallThickness();

	bool valueTransform();
	bool extractVOI();
	bool thresholdImage();
	bool edgeDetection();
	bool thicknessCal();
	bool setExtent(int* extent);
	const int* getExtent();

//private:

	vtkSmartPointer<vtkImageData> image;
	vtkSmartPointer<vtkImageData> edgeImage[EDGENUM];


	int edgeValue[EDGENUM];
	int* extent;

	struct EdgePoint {
		double x = 0;
		double y = 0;
	};


	EdgePoint* centers[EDGENUM];
	std::list<EdgePoint>* edgePoints[EDGENUM];

};

