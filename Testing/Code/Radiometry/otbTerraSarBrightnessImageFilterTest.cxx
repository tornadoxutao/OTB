/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"

#include "otbTerraSarBrightnessImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


int otbTerraSarBrightnessImageFilterTest(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef otb::Image<double, 2>                                    ImageType;
  typedef otb::ImageFileReader<ImageType>                          ReaderType;
  typedef otb::ImageFileWriter<ImageType>                          WriterType;
  typedef otb::TerraSarBrightnessImageFilter<ImageType, ImageType> FilterType;


  ReaderType::Pointer reader         = ReaderType::New();
  WriterType::Pointer writer         = WriterType::New();

  FilterType::Pointer filter         = FilterType::New();

  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->UpdateOutputInformation();

  //std::cout<<reader->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;

  if( atoi(argv[3]) == 1 )
    filter->SetCalFactor( 10 );

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
