#ifndef MAXIMUMWALLTHICKNESS_H
#define MAXIMUMWALLTHICKNESS_H
#endif // !MAXIUMWALLTHICKNESS_H

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkImageShiftScale.h>
#include <vtkImageTHreshold.h>
#include <vtkExtractVOI.h>
#include <vtkThreshold.h>


struct edgePoint;

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
	bool setExtent(int* extent);
	const int* getExtent();

//private:

	vtkSmartPointer<vtkImageData> image;
	vtkSmartPointer<vtkImageData> edgeImage[EDGENUM];


	int edgeValue[EDGENUM];
	int* extent;

};

struct edgePoint {
	int x = 0;
	int y = 0;

};
