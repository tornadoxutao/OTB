/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbComputeHistoFilter_txx
#define otbComputeHistoFilter_txx

#include "otbComputeHistoFilter.h"
#include "itkImageSource.h"
#include "itkImageRegionIterator.h"

#include <limits>

// #define DEBUG
namespace otb
{

template <class TInputImage, class TOutputImage >
ComputeHistoFilter < TInputImage , TOutputImage >
::ComputeHistoFilter()
{
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput( 0, this->MakeOutput(0) );
  this->SetNthOutput( 1, this->MakeOutput(1) );
  //m_TargetHisto =  OutputImageType::New() ;
  m_Min = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_Max = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_NoData = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_NbBin = 256;
  m_Threshold = std::numeric_limits< float >::max();
  m_ThumbSize.Fill(-1);
}

template <class TInputImage, class TOutputImage >
itk::DataObject::Pointer ComputeHistoFilter < TInputImage , TOutputImage >
::MakeOutput(unsigned int idx)
{
  itk::DataObject::Pointer output;
 
  switch ( idx )
    {
    case 0:
      output = ( OutputImageType::New() ).GetPointer();
      break;
    case 1:
      output = ( OutputImageType::New() ).GetPointer();
      break;
    default:
      std::cerr << "No output " << idx << std::endl;
      output = NULL;
      break;
    }
  return output.GetPointer();
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateInputRequestedRegion()
{
  #ifdef DEBUG
  std::cout<<"##################GenerateInputRequestedRegion################"<<std::endl;
  #endif
  // Same region as the virtual output : when bug corrected factorize this!!!
  assert( m_ThumbSize[0]>0);
  assert( m_ThumbSize[1]>0);

  // Superclass::GenerateInputRequestedRegion();
  typename Superclass::InputImagePointer inputPtr =
                  const_cast<InputImageType *>(this->GetInput());
  IndexType start ;
  start[0] = this->GetHistoOutput()->GetRequestedRegion().GetIndex()[0] * m_ThumbSize[0];
  start[1] = this->GetHistoOutput()->GetRequestedRegion().GetIndex()[1] * m_ThumbSize[1];
  SizeType size ;
  size[0] = this->GetHistoOutput()->GetRequestedRegion().GetSize()[0] * m_ThumbSize[0];
  size[1] = this->GetHistoOutput()->GetRequestedRegion().GetSize()[1] * m_ThumbSize[1];

  #ifdef DEBUG
  std::cout<<"start "<<start<<std::endl;
  std::cout<<"size "<<size<<std::endl;
  std::cout<<"Requested Index  "<<this->GetHistoOutput()->GetRequestedRegion().GetIndex()<<std::endl;
  std::cout<<"Requested Size "<<this->GetHistoOutput()->GetRequestedRegion().GetSize()<<std::endl;
  #endif

  typename InputImageType::RegionType inputRequestedRegion;
  inputRequestedRegion.SetIndex(start);
  inputRequestedRegion.SetSize(size);

  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
  #ifdef DEBUG
  std::cout<<"##################End GenerateInputRequestedRegion################"<<std::endl;
  #endif
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateOutputInformation()
{
  #ifdef DEBUG
  std::cout<<"###############GenerateOutputInformation###############"<<std::endl;
  #endif

  Superclass::GenerateOutputInformation();
  typename InputImageType::ConstPointer input = this->GetInput();
  typename OutputImageType::Pointer output = this->GetHistoOutput();

  if ( !input || !output )
    {
    return;
    }

  typename OutputImageType::IndexType start;
  typename OutputImageType::SizeType size;
  start[0] = 0;
  start[1] = 0;
  size[0] = input->GetLargestPossibleRegion().GetSize()[0]/m_ThumbSize[0];
  size[1] = input->GetLargestPossibleRegion().GetSize()[1]/m_ThumbSize[1];
  typename OutputImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  output->SetNumberOfComponentsPerPixel(m_NbBin);
  output->SetLargestPossibleRegion(region);

  typename OutputImageType::Pointer outImage = this->GetOutput();
  outImage->SetLargestPossibleRegion(input->GetLargestPossibleRegion());
  #ifdef DEBUG
  std::cout<<"###############End GenerateOutputInformation###############"<<std::endl;
  #endif
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateOutputRequestedRegion( itk::DataObject *output )
{
  #ifdef DEBUG
  std::cout<<"###########GenerateOutputRequestedRegion###########"<<std::endl;
  std::cout<<"Requested Output index : "<<this->GetHistoOutput()->GetRequestedRegion().GetIndex()<<std::endl;
  std::cout<<"Requested Output size : "<<this->GetHistoOutput()->GetRequestedRegion().GetSize()<<std::endl;
  #endif

  typename OutputImageType::Pointer outImage = this->GetOutput();
  IndexType start ;
  start[0] = this->GetHistoOutput()->GetRequestedRegion().GetIndex()[0] * m_ThumbSize[0];
  start[1] = this->GetHistoOutput()->GetRequestedRegion().GetIndex()[1] * m_ThumbSize[1];
  SizeType size ;
  size[0] = this->GetHistoOutput()->GetRequestedRegion().GetSize()[0] * m_ThumbSize[0];
  size[1] = this->GetHistoOutput()->GetRequestedRegion().GetSize()[1] * m_ThumbSize[1];
  typename OutputImageType::RegionType inputRequestedRegion;
  inputRequestedRegion.SetIndex(start);
  inputRequestedRegion.SetSize(size);
  if (inputRequestedRegion.Crop(outImage->GetLargestPossibleRegion()))
    {
    outImage->SetRequestedRegion(inputRequestedRegion);
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    outImage->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(outImage);
    throw e;
    }
  #ifdef DEBUG
  std::cout<<"###########End GenerateOutputRequestedRegion###########"<<std::endl;
  #endif
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateData()
{
  #ifdef DEBUG
  std::cout<<"##############GenerateData################"<<std::endl;
  std::cout<<"output requested "<<this->GetHistoOutput()->GetRequestedRegion().GetSize()<<std::endl;
  std::cout<<"output Largest "<<this->GetHistoOutput()->GetLargestPossibleRegion().GetSize()<<std::endl;
  std::cout<<"output Buffered "<<this->GetHistoOutput()->GetBufferedRegion().GetSize()<<std::endl;
  #endif
  this->AllocateOutputs();
  #ifdef DEBUG
  std::cout<<"After Allocate"<<std::endl;
  std::cout<<"output requested "<<this->GetHistoOutput()->GetRequestedRegion().GetSize()<<std::endl;
  std::cout<<"output Largest "<<this->GetHistoOutput()->GetLargestPossibleRegion().GetSize()<<std::endl;
  std::cout<<"output Buffered "<<this->GetHistoOutput()->GetBufferedRegion().GetSize()<<std::endl;
  #endif

  // Set up the multithreaded processing
  typename itk::ImageSource<OutputImageType>::ThreadStruct str;
  str.Filter = this;

  // Get the output pointer
  const OutputImageType *outputPtr = this->GetOutput();
  const itk::ImageRegionSplitterBase * splitter = this->GetImageRegionSplitter();
  m_ValidThreads = 
    splitter->GetNumberOfSplits( outputPtr->GetRequestedRegion() , 
                                 this->GetNumberOfThreads() );

  this->BeforeThreadedGenerateData();

  this->GetMultiThreader()->SetNumberOfThreads( m_ValidThreads );
  this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);
  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  this->AfterThreadedGenerateData();
  #ifdef DEBUG
  std::cout<<"##############End GenerateData################"<<std::endl;
  #endif
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::BeforeThreadedGenerateData()
{
  #ifdef DEBUG
  std::cout<<"##############BeforeThreadedGenerateData################"<<std::endl;
  #endif
  // Initializing output
  typename OutputImageType::Pointer output = this->GetHistoOutput();
  typename OutputImageType::PixelType zeroPixel ; 
  zeroPixel.SetSize( m_NbBin );
  zeroPixel.Fill(0);
  #ifdef DEBUG
  std::cout<<"Vector length "<<output->GetVectorLength()<<std::endl;
  std::cout<<"zeroPixel length "<<zeroPixel.GetSize()<<std::endl;
  #endif
  output->FillBuffer( zeroPixel );

  // Initializing shared variable with thread number parameter
  SizeType outSize = output->GetRequestedRegion().GetSize();
  m_HistoThread.resize( m_ValidThreads * outSize[0] * outSize[1] , zeroPixel );

  #ifdef DEBUG
  std::cout<<"NbThread "<<m_ValidThreads<<std::endl;
  std::cout<<"vector size "<<m_HistoThread.size()<<std::endl;
  std::cout<<"Vector's pixel "<<m_HistoThread[0]<<std::endl;
  #endif

  m_Step = static_cast<double>( m_Max - m_Min ) \
                / static_cast<double>( m_NbBin -1 );
  #ifdef DEBUG
  std::cout<<"##############End BeforeThreadedGenerateData################"<<std::endl;
  #endif
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread ,
                       itk::ThreadIdType threadId )
{
  #ifdef DEBUG
  std::cout<<"##############ThreadedGenerateData################"<<std::endl;
  std::cout<<"ID : "<<threadId<<std::endl;
  #endif
  itk::ProgressReporter progress(this , threadId , 
                outputRegionForThread.GetNumberOfPixels() );
  typename InputImageType::ConstPointer input = this->GetInput();
  OutputImageRegionType histoRegion = 
                            GetHistoOutput()->GetRequestedRegion();
  SizeType outSize = histoRegion.GetSize();
  IndexType outIndex = histoRegion.GetIndex();
  int threadIndex = ( threadId ) * outSize[0] * outSize[1];
  int pixel = 0;
  typename InputImageType::RegionType region;
  region.SetSize(m_ThumbSize);
  for ( unsigned int nthHisto = 0 ; nthHisto < outSize[0] * outSize[1] ; nthHisto++ )
    {
    IndexType start;
    start[0] = (outIndex[0] +  nthHisto % outSize[0] ) * m_ThumbSize[0];
    start[1] = (outIndex[1] +  nthHisto / outSize[0] ) * m_ThumbSize[1];
    region.SetIndex(start);
    typename itk::ImageRegionConstIterator < InputImageType > 
      it( input ,region );
    it.GoToBegin();
    while ( !it.IsAtEnd() )
      {
      if( it.Get() == m_NoData || it.Get() > m_Max || it.Get() < m_Min )
        {
        ++it;
        continue;
        }
      pixel = static_cast<int>( std::round( ( it.Get() - m_Min ) / m_Step ) );
      ++m_HistoThread[threadIndex + nthHisto][pixel];
      ++it;
      }
    }
  #ifdef DEBUG
  std::cout<<"##############End ThreadedGenerateData################"<<std::endl;
  #endif
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::AfterThreadedGenerateData()
{
  #ifdef DEBUG
  std::cout<<"after histo"<<std::endl;
  #endif
  itk::ThreadIdType const nbThread = this->GetNumberOfThreads();
  typename OutputImageType::Pointer output = this->GetHistoOutput();
  typename itk::ImageRegionIterator < OutputImageType > 
      oit( output , output->GetRequestedRegion() );
  SizeType outSize = this->GetHistoOutput()->GetRequestedRegion().GetSize();
  IndexType outIndex = this->GetHistoOutput()->GetRequestedRegion().GetIndex();
  int agreg = 0;
  int total = 0;
  int height = 0;
  int rest = 0;
  oit.GoToBegin();
  while ( !oit.IsAtEnd() )
    {
    total = 0;
    for (int i = 0 ; i<m_NbBin ; i++)
      {
      agreg = 0;

      for (unsigned int threadId = 0 ; threadId<nbThread ; threadId++ )
        {
        agreg += m_HistoThread[threadId * outSize[0] * outSize[1] \
          + ( ( oit.GetIndex()[0] - outIndex[0] )  ) \
          + ( oit.GetIndex()[1] - outIndex[1] ) * outSize[0]][i]; 
          // std::cout<<threadId * outSize[0] * outSize[1] + ( ( oit.GetIndex()[0] - outIndex[0] )  )  + ( oit.GetIndex()[1] - outIndex[1] ) * outSize[0]<<std::endl;
          // std::cout<<agreg<<std::endl;
        }
      oit.Get()[i] = agreg;
      total += agreg;
      }
    height = 0;
    rest = 0;
    if ( m_Threshold == std::numeric_limits< float >::max() )
      {
      ++oit;
      continue;
      }
    height = static_cast<int>( m_Threshold * ( total / m_NbBin ) );
    // Warning!!!! Need to handle out of bound int!!!
    for( int i = 0 ; i < m_NbBin ; i++ )
      {
      if ( oit.Get()[i] > height )
        {
        rest += oit.Get()[i] - height ;
        oit.Get()[i] = height ;
        }
      }
    height = rest / m_NbBin;
    rest = rest % m_NbBin;
    for( int i = 0 ; i < m_NbBin ; i++ )
      {
      oit.Get()[i] += height ;
      if ( i > (m_NbBin - rest)/2 && i <= (m_NbBin - rest)/2 + rest )
        {
        ++oit.Get()[i];
        }
      }
    ++oit;
    }
}

template <class TInputImage, class TOutputImage >
typename TOutputImage::Pointer ComputeHistoFilter < TInputImage , TOutputImage >
::GetHistoOutput()
{
  #ifdef DEBUG
  std::cout<<"*******GetHistoOutput*******"<<std::endl;
  #endif
  return dynamic_cast< TOutputImage * >(
           this->itk::ProcessObject::GetOutput(1) );
}
  
} // End namespace otb

#endif