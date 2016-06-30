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

	return true;
}

bool MaximumWallThickness::thicknessCal()
{
	using namespace std;


	for (int num = 0; num < EDGENUM; num++) {

		list<EdgePoint>* edgePointer = edgePoints[num];
		EdgePoint* centersPointer = centers[num];

		for (int k = extent[4]; k <= extent[5]; k++) {

			edgePointer->clear();
			
			int centersX = 0;
			int centersY = 0;
			for (int j = extent[2]; j <= extent[3]; j++) {
				for (int i = extent[0]; i <= extent[1]; i++) {
					int* pointer = static_cast<int*>(edgeImage[num]->GetScalarPointer(i, j, k));
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
			//cout << "edgePointers number:" << edgePointer->size() << endl;
			EdgePoint e;
			//if(edgePointer->size() != 0){
				e.x = (double)centersX / edgePointer->size();
				e.y = (double)centersY / edgePointer->size();
			//}
			*centersPointer = e;
			++centersPointer;
			++edgePointer;

		}

	}


	list<EdgePoint>* internalEdgePointer = edgePoints[0];
	list<EdgePoint>* externalEdgePointer = edgePoints[1];
	EdgePoint* internalCenterPointer = centers[0];

	maximumWallThickness.clear();

	
	for (int k = extent[4]; k <= extent[5]; k++) {

		list<pair<double, pair<EdgePoint, EdgePoint>>> wallThickness;
		for (list<EdgePoint>::const_iterator internalEdgeIt = internalEdgePointer->cbegin(); 
			internalEdgeIt != internalEdgePointer->cend(); ++internalEdgeIt) {
			double kRL = (internalEdgeIt->y - internalCenterPointer->y) / 
				(internalEdgeIt->x - internalCenterPointer->x);
			double bRL = (internalEdgeIt->x * internalCenterPointer->y - 
				internalEdgeIt->y * internalCenterPointer->x) / (internalEdgeIt->x - internalCenterPointer->x);
			
			list<pair<double, EdgePoint>> distancePL;
			for (list<EdgePoint>::const_iterator externalEdgeIt = externalEdgePointer->cbegin();
				externalEdgeIt != externalEdgePointer->cend(); ++externalEdgeIt) {
				double temp = abs(kRL * externalEdgeIt->x - externalEdgeIt->y + bRL);
				temp = temp / sqrt(pow(kRL, 2) + 1);
				
				distancePL.push_back(pair<double, EdgePoint>(temp, *externalEdgeIt));


			}
			distancePL.sort();

			for (list<pair<double, EdgePoint>>::const_iterator distancePLIt = distancePL.cbegin();
				distancePLIt != distancePL.cend();++distancePLIt) {
				if ((internalEdgeIt->x > internalCenterPointer->x) == (distancePLIt->second.x > internalCenterPointer->x) &&
					(internalEdgeIt->y > internalCenterPointer->y) == (distancePLIt->second.y > internalCenterPointer->y)) {
					double temp;
					temp = pow((internalEdgeIt->x - distancePLIt->second.x), 2);
					temp += pow((internalEdgeIt->y - distancePLIt->second.y), 2);
					temp = sqrt(temp);
					pair<double, pair<EdgePoint, EdgePoint>> p1(temp,
						pair<EdgePoint, EdgePoint>(*internalEdgeIt, distancePLIt->second));
					//pair<EdgePoint, EdgePoint> p;
					//p.first = *internalEdgeIt;
					//p.second = distancePLIt->second;
					//pair<double, pair<EdgePoint, EdgePoint>> p1;
					//p1.first = temp;
					//p1.second = p;
					wallThickness.push_back(p1);
					break;
				}
			}
		}	
		wallThickness.sort();
		maximumWallThickness.push_back(wallThickness.back());
		

		//cout << "slice " << k << " " << wallThickness.back().first << endl;
		//cout << "internal" << wallThickness.back().second.first.x << '\t' << wallThickness.back().second.first.y << endl;
		//cout << "external" << wallThickness.back().second.second.x << '\t' << wallThickness.back().second.second.y << endl;
		wallThickness.clear();

		++internalEdgePointer;
		++externalEdgePointer;
		++internalCenterPointer;
	}



	return true;
}

bool MaximumWallThickness::output()
{
	using namespace std;
	int k = extent[4];
	for (list<pair<double, pair<EdgePoint, EdgePoint>>>::const_iterator maximumWallThicknessIt = 
		maximumWallThickness.cbegin(); maximumWallThicknessIt != maximumWallThickness.cend(); ++maximumWallThicknessIt) {
		cout << "slice" << k++ << endl;
		cout << "distance: " << maximumWallThicknessIt->first << endl;
		cout << "internal: " << maximumWallThicknessIt->second.first.x << '\t' <<
			maximumWallThicknessIt->second.first.y << endl;
		cout << "external: " << maximumWallThicknessIt->second.second.x << '\t' <<
			maximumWallThicknessIt->second.second.y << endl;
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

