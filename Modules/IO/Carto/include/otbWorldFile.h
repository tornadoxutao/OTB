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

#ifndef otbWorldFile_h
#define otbWorldFile_h

#include <iomanip>
#include <fstream>

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb {

/**
 * \class WorldFile
 * \brief Handles the world file that associates geographic information to png, jpg
 *
 *  The world file format is detailed at
 *  http://en.wikipedia.org/wiki/World_file
 *
 *
 * \ingroup OTBCarto
 */

class ITK_EXPORT WorldFile : public itk::Object
{
public:

  typedef WorldFile                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  itkGetMacro(LonOrigin, double)
  itkSetMacro(LonOrigin, double)
  itkGetMacro(LatOrigin, double)
  itkSetMacro(LatOrigin, double)
  itkGetMacro(LonSpacing, double)
  itkSetMacro(LonSpacing, double)
  itkGetMacro(LatSpacing, double)
  itkSetMacro(LatSpacing, double)
  itkGetMacro(LonRotation, double)
  itkSetMacro(LonRotation, double)
  itkGetMacro(LatRotation, double)
  itkSetMacro(LatRotation, double)

  itkGetStringMacro(ImageFilename)
  itkSetStringMacro(ImageFilename)

  void Update()
  {
    if (m_ImageFilename.empty())
      {
      itkExceptionMacro(<< "The image filename must be provided");
      }

    std::string worldFilename;
    int         i = m_ImageFilename.find_last_of('.');
    worldFilename = m_ImageFilename.substr(0, i) + ".wld";

    std::ofstream file;
    file.open(worldFilename.c_str());
    file << std::setprecision(15);
    file << m_LonSpacing << std::endl;
    file << m_LatRotation << std::endl; //yes, in this order
    file << m_LonRotation << std::endl;
    file << m_LatSpacing << std::endl;
    file << m_LonOrigin << std::endl;
    file << m_LatOrigin << std::endl;
    file.close();
  }

protected:
  WorldFile() : m_LonOrigin(0.0), m_LatOrigin(0.0),
    m_LonSpacing(0.0), m_LatSpacing(0.0),
    m_LonRotation(0.0), m_LatRotation(0.0),
    m_ImageFilename("") {}
  ~WorldFile() override {}

private:
  WorldFile(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  double      m_LonOrigin;
  double      m_LatOrigin;
  double      m_LonSpacing;
  double      m_LatSpacing;
  double      m_LonRotation;
  double      m_LatRotation;
  std::string m_ImageFilename;
};
} // end namespace otb

#endif
