#include "MaximumWallThickness.h"


MaximumWallThickness::MaximumWallThickness(vtkImageData * image, int internalEdgeValue, int externalEdgeValue)
	:image(image)
{
	edgeValue[0] = (internalEdgeValue);
	edgeValue[1] = externalEdgeValue;
	this->extent = image->GetExtent();
	
	for (int num = 0; num < EDGENUM; ++num) {
		edgePoints[num] = new std::list<EdgePoint>[extent[5] - extent[4] + 1];
		centers[num] = new EdgePoint[extent[5] - extent[4] + 1];
	}
	
	
}

MaximumWallThickness::~MaximumWallThickness()
{
	for (int num = 0; num < EDGENUM; ++num) {
		delete[] edgePoints[num];
		delete[] centers[num];
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

	for (int k = extent[4]; k <= extent[5]; ++k) {
		double kRL;
		double bRL;


	}
	return true;
}

bool MaximumWallThickness::thicknessCal()
{
	using namespace std;

	for (int num = 0; num < EDGENUM; num++) {

		int* pointer;
		
		list<EdgePoint>* edgePointer = edgePoints[num];
		EdgePoint* centersPointer = centers[num];

		for (int k = extent[4]; k <= extent[5]; k++) {
			int centersX = 0;
			int centersY = 0;
			for (int j = extent[2]; j <= extent[3]; j++) {
				for (int i = extent[0]; i <= extent[1]; i++) {
					pointer = static_cast<int*>(edgeImage[num]->GetScalarPointer(i, j, k));
					if (*pointer != image->GetScalarTypeMin()) {
						EdgePoint e;
						e.x = i;
						e.y = j;
						centersX += i;
						centersY += j;
						edgePointer->push_back(e);
					}
					
				}
			}
			//cout << edgePointer->size()<<endl;
			EdgePoint e;
			//if(edgePointer->size() != 0){
				e.x = (double)centersX / edgePointer->size();
				e.y = (double)centersY / edgePointer->size();
			//}
			*centersPointer = e;
			++centersPointer;
			++edgePointer;

		}
		//for (int k = extent[4]; k <= extent[5]; k++) {
		//	cout << centersPointer->x<<'\n';
		//	++centersPointer;
		//}

	}

	list<EdgePoint>* internalEdgePointer = edgePoints[0];
	list<EdgePoint>* externalEdgePointer = edgePoints[1];
	EdgePoint* internalCenterPointer = centers[0];
	list<double> maximumWallThickness;
	
	for (int k = extent[4]; k <= extent[5]; k++) {
		list<EdgePoint>::const_iterator internalEdgeIt = internalEdgePointer->cbegin();
		for (list<EdgePoint>::const_iterator internalEdgeIt = internalEdgePointer->cbegin(); 
			internalEdgeIt != internalEdgePointer->cend(); ++internalEdgeIt) {
			double kRL = (internalEdgeIt->y - internalCenterPointer->y) / 
				(internalEdgeIt->x - internalCenterPointer->y);
			double bRL = (internalEdgeIt->x * internalCenterPointer->y - 
				internalEdgeIt->y * internalCenterPointer->x) / (internalEdgeIt->x - internalCenterPointer->y);
			
			list<pair<double, EdgePoint>> distancePL;
			for (list<EdgePoint>::const_iterator externalEdgeIt = externalEdgePointer->cbegin();
				externalEdgeIt != externalEdgePointer->cend(); ++externalEdgeIt) {
				double temp = abs(kRL * externalEdgeIt->x - externalEdgeIt->y + bRL);
				temp = temp / sqrt(pow(kRL, 2) + 1);
				
				distancePL.push_back(pair<double, EdgePoint>(temp, *externalEdgeIt));


			}
			distancePL.sort();
			for(list<pair<double, EdgePoint>>::const_iterator)
			
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

