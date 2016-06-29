#pragma once

#include <itkImageFileReader.h>
#include <itkSmartPointer.h>
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

class processor
{
public:
	processor();
	~processor();

	static itk::Image<unsigned int, 3>::Pointer itkLoadFile(const std::string fileName);
	static vtkSmartPointer<vtkImageData> itk2Vtk(itk::Image<unsigned int, 3>::Pointer img);
};

