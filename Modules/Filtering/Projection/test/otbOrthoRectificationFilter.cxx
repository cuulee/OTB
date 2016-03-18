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


// iostream is used for general output
#include <iostream>
#include <stdlib.h>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMapProjections.h"
#include "itkUnaryFunctorImageFilter.h"
//#include "itkComplexToModulusImageFilter.h"
#include "otbDEMHandler.h"
#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"

int otbOrthoRectificationFilter(int argc, char* argv[])
{
  if (argc != 14)
    {
    std::cout << argv[0] <<
    " <input filename> <output filename> <origin easting> <origin northing>"
    " <x size> <y size> <x spacing> <y spacing> <UTM zone> <UTM hemisphere>"
    " <grid_spacing> <mode> <mode.info>"
              << std::endl;

    return EXIT_FAILURE;
    }

  typedef otb::VectorImage<double, 2>                                               VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>                                     ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>                            WriterType;
  typedef otb::UtmInverseProjection                                                 UtmMapProjectionType;
  typedef otb::OrthoRectificationFilter<VectorImageType, VectorImageType, UtmMapProjectionType> OrthoRectifFilterType;

  //Allocate pointer
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();
  UtmMapProjectionType::Pointer  utmMapProjection = UtmMapProjectionType::New();

  // Set parameters ...
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  reader->GenerateOutputInformation();
  std::cout << reader->GetOutput() << std::endl;

  orthoRectifFilter->SetInput(reader->GetOutput());

  VectorImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  orthoRectifFilter->SetOutputStartIndex(start);

  VectorImageType::SizeType size;
  size[0] = atoi(argv[5]);      // X size
  size[1] = atoi(argv[6]);            //Y size
  orthoRectifFilter->SetOutputSize(size);

  VectorImageType::SpacingType spacing;
  spacing[0] = atof(argv[7]);
  spacing[1] = atof(argv[8]);
  orthoRectifFilter->SetOutputSpacing(spacing);

  VectorImageType::PointType origin;
  origin[0] = strtod(argv[3], NULL);         //Origin easting
  origin[1] = strtod(argv[4], NULL);         //Origin northing
  orthoRectifFilter->SetOutputOrigin(origin);

  utmMapProjection->SetZone(atoi(argv[9]));
  utmMapProjection->SetHemisphere(argv[10][0]);
  orthoRectifFilter->SetMapProjection(utmMapProjection);

  // Displacement Field spacing
  VectorImageType::SpacingType  gridSpacing;
  gridSpacing[0] = atof(argv[11]);
  gridSpacing[1] = -atof(argv[11]);
  orthoRectifFilter->SetDisplacementFieldSpacing(gridSpacing);

  VectorImageType::PixelType no_data(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  no_data.Fill(0);
  orthoRectifFilter->SetEdgePaddingValue(no_data);
  
  // manage demHandler
  if (atoi(argv[12])==1) //mode = no DEM
  {
	  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(135.8);
  }
  else if ( (atoi(argv[12])==2) || (atoi(argv[12])==3) ) //mode = DEM SRTM || DEM GTIFF
  {
	  otb::DEMHandler::Instance()->OpenDEMDirectory(argv[13]);
  }

  writer->SetInput(orthoRectifFilter->GetOutput());
  writer->SetNumberOfDivisionsTiledStreaming(4);
  writer->Update();

  return EXIT_SUCCESS;
}
