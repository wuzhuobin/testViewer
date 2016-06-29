#include "processor.h"



processor::processor()
{
}


processor::~processor()
{
}

itk::Image<unsigned int, 3>::Pointer processor::itkLoadFile(const std::string fileName)
{
	itk::ImageFileReader<itk::Image<unsigned int, 3>>::Pointer reader =
		itk::ImageFileReader<itk::Image<unsigned int, 3>>::New();
	reader->SetFileName(fileName);
	reader->Update();


	return reader->GetOutput();
}

vtkSmartPointer<vtkImageData> processor::itk2Vtk(itk::Image<unsigned int, 3>::Pointer img)
{
	itk::ImageToVTKImageFilter<itk::Image<unsigned int, 3>>::Pointer filter =
		itk::ImageToVTKImageFilter<itk::Image<unsigned int, 3>>::New();
	filter->SetInput(img);
	filter->Update();
	vtkSmartPointer<vtkImageData> img1 =
		vtkSmartPointer<vtkImageData>::New();
	img1->DeepCopy(filter->GetOutput());
	return img1;

}
