
#include "itkImage.h"
#include <iostream>
#include "itkFastGrowCut.h"
#include "itkImageFileReader.h"
#include "itkCommand.h"
#include "itkMedianImageFilter.h"
#include "itkImageFileWriter.h"

namespace
{
	class ShowProgress : public itk::Command
	{
	public:
		itkNewMacro(ShowProgress);

		void
			Execute(itk::Object* caller, const itk::EventObject& event) override
		{
			Execute((const itk::Object*)caller, event);
		}

		void
			Execute(const itk::Object* caller, const itk::EventObject& event) override
		{
			if (!itk::ProgressEvent().CheckEvent(&event))
			{
				return;
			}
			const auto* processObject = dynamic_cast<const itk::ProcessObject*>(caller);
			if (!processObject)
			{
				return;
			}
			std::cout << " " << processObject->GetProgress();
		}
	};
} // namespace

int main()
{
	constexpr unsigned int Dimension = 3;
	using PixelType = short;
	using ImageType = itk::Image<PixelType, Dimension>;
	using LabelType = itk::Image<unsigned char, Dimension>;

	using FGCType = itk::FastGrowCut<ImageType, LabelType>;
	FGCType::Pointer fgcFilter = FGCType::New();

	std::string imageFilename = "C:\\Users\\DELL\\Desktop\\growcutdata\\input_original.mhd";
	std::string maskFilename = "C:\\Users\\DELL\\Desktop\\growcutdata\\brush.mhd";
	std::string outputFilename = "C:\\Users\\DELL\\Desktop\\growcutdata\\growcut_result.mhd";

	// Read the image
	std::cout << "*** Reading image ***" << std::endl;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(imageFilename);
	reader->Update();

	// Read the seed masks
	std::cout << "*** Reading foreground mask ***" << std::endl;
	typedef itk::ImageFileReader<LabelType> ForegroundMaskReaderType;
	ForegroundMaskReaderType::Pointer foregroundMaskReader = ForegroundMaskReaderType::New();
	foregroundMaskReader->SetFileName(maskFilename);
	foregroundMaskReader->Update();

	ShowProgress::Pointer showProgress = ShowProgress::New();
	fgcFilter->AddObserver(itk::ProgressEvent(), showProgress);
	fgcFilter->SetInput(reader->GetOutput());
	fgcFilter->SetSeedImage(foregroundMaskReader->GetOutput());
	fgcFilter->Update();

	typedef itk::ImageFileWriter<LabelType> WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(outputFilename);
	writer->SetInput(fgcFilter->GetOutput());
	writer->Update();
	return EXIT_SUCCESS;
}

