#include "MaximumWallThickness.h"


MaximumWallThickness::MaximumWallThickness(vtkImageData * image, int internalEdgeValue, int externalEdgeValue)
	:image(image)
{
	edgeValue[0] = (internalEdgeValue);
	edgeValue[1] = externalEdgeValue;
	this->extent = image->GetExtent();
	
	
}

MaximumWallThickness::~MaximumWallThickness()
{
}

bool MaximumWallThickness::valueTransform()
{
	vtkSmartPointer<vtkImageShiftScale> shiftScaleFilter =
		vtkSmartPointer<vtkImageShiftScale>::New();
	shiftScaleFilter->SetInputData(image);
	shiftScaleFilter->SetOutputScalarType(VTK_INT);
	shiftScaleFilter->Update();

	//vtkSmartPointer<vtkImageThreshold> thresholdFilter =
	//	vtkSmartPointer<vtkImageThreshold>::New();


	return true;
}

bool MaximumWallThickness::extractVOI()
{
	vtkSmartPointer<vtkExtractVOI> extractFilter =
		vtkSmartPointer<vtkExtractVOI>::New();
	extractFilter->SetInputData(image);
	extractFilter->SetVOI(extent);
	extractFilter->Update();
	image = extractFilter->GetOutput();
	return false;
}

bool MaximumWallThickness::thresholdImage()
{
	for (int num = 0; num < EDGENUM; ++num) {
		edgeImage[num] = vtkSmartPointer<vtkImageData>::New();
		edgeImage[num]->DeepCopy(image);
		vtkSmartPointer<vtkImageThreshold> thresholdFilter =
			vtkSmartPointer<vtkImageThreshold>::New();
		thresholdFilter->SetInputData(edgeImage[num]);
		thresholdFilter->ThresholdBetween(edgeValue[num], edgeValue[num]);
		thresholdFilter->SetInValue(image->GetScalarTypeMax());
		thresholdFilter->SetOutValue(image->GetScalarTypeMin());
		thresholdFilter->Update();
		edgeImage[num] = thresholdFilter->GetOutput();
		
	}
	

	return true;
}

bool MaximumWallThickness::edgeDetection()
{
	return false;
}

bool MaximumWallThickness::setExtent(int* extent)
{

	this->extent = extent;
	return true;
}

const int * MaximumWallThickness::getExtent()
{
	return extent;
}

