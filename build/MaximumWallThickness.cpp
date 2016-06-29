#include "MaximumWallThickness.h"


MaximumWallThickness::MaximumWallThickness(vtkImageData * image, int internalEdgeValue, int externalEdgeValue)
	:image(image)
{
	edgeValue[0] = (internalEdgeValue);
	edgeValue[1] = externalEdgeValue;
	this->extent = image->GetExtent();
	
	for (int num = 0; num < EDGENUM; ++num) {
		edgePoints[num] = new std::list<EdgePoint>[extent[5] - extent[4] + 1];
	}
	
	
}

MaximumWallThickness::~MaximumWallThickness()
{
	for (int num = 0; num < EDGENUM; ++num) {
		delete[] edgePoints[num];
	}

}

bool MaximumWallThickness::valueTransform()
{
	vtkSmartPointer<vtkImageShiftScale> shiftScaleFilter =
		vtkSmartPointer<vtkImageShiftScale>::New();
	shiftScaleFilter->SetInputData(image);
	shiftScaleFilter->SetOutputScalarType(VTK_INT);
	shiftScaleFilter->Update();


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

	return true;
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
	for (int num = 0; num < EDGENUM; ++num) {
		vtkSmartPointer<vtkImageSobel2D> sobelFilter =
			vtkSmartPointer<vtkImageSobel2D>::New();
		sobelFilter->SetInputData(edgeImage[num]);
		sobelFilter->Update();

		vtkSmartPointer<vtkImageExtractComponents> extractXFilter =
			vtkSmartPointer<vtkImageExtractComponents>::New();
		extractXFilter->SetComponents(0);
		extractXFilter->SetInputConnection(sobelFilter->GetOutputPort());
		extractXFilter->Update();

		vtkSmartPointer<vtkImageMathematics> xImageSquare =
			vtkSmartPointer<vtkImageMathematics>::New();
		xImageSquare->SetOperationToSquare();
		xImageSquare->SetInputConnection(extractXFilter->GetOutputPort());
		xImageSquare->Update();

		vtkSmartPointer<vtkImageExtractComponents> extractYFilter =
			vtkSmartPointer<vtkImageExtractComponents>::New();
		extractYFilter->SetComponents(1);
		extractYFilter->SetInputConnection(sobelFilter->GetOutputPort());
		extractYFilter->Update();

		vtkSmartPointer<vtkImageMathematics> yImageSquare =
			vtkSmartPointer<vtkImageMathematics>::New();
		yImageSquare->SetOperationToSquare();
		yImageSquare->SetInputConnection(extractYFilter->GetOutputPort());
		yImageSquare->Update();

		vtkSmartPointer<vtkImageMathematics> imageAdd =
			vtkSmartPointer<vtkImageMathematics>::New();
		imageAdd->SetOperationToAdd();
		imageAdd->SetInput1Data(xImageSquare->GetOutput());
		imageAdd->SetInput2Data(yImageSquare->GetOutput());
		imageAdd->Update();

		vtkSmartPointer<vtkImageMathematics> imageSqrt =
			vtkSmartPointer<vtkImageMathematics>::New();
		imageSqrt->SetOperationToSquareRoot();
		imageSqrt->SetInputConnection(imageAdd->GetOutputPort());
		imageSqrt->Update();

		edgeImage[num] = imageSqrt->GetOutput();

	}
	return true;
}

bool MaximumWallThickness::thicknessCal()
{
	using namespace std;
	for (int num = 0; num < EDGENUM; num++) {

		int* pointer;
		
		list<EdgePoint>* edgePointer = edgePoints[num];

		for (int k = extent[4]; k <= extent[5]; k++) {
			for (int j = extent[2]; j <= extent[3]; j++) {
				for (int i = extent[0]; i <= extent[1]; i++) {
					pointer = static_cast<int*>(edgeImage[num]->GetScalarPointer(i, j, k));
					if (*pointer != image->GetScalarTypeMin()) {
						EdgePoint e;
						e.x = i;
						e.y = j;
						edgePointer->push_back(e);
					}
				}
			}

			++edgePointer;

		}
	}

	return true;
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

