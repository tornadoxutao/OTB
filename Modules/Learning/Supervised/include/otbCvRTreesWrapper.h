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

#ifndef otbCvRTreesWrapper_h
#define otbCvRTreesWrapper_h

#include "otbOpenCVUtils.h"
#include <vector>

namespace otb
{

/** \class CvRTreesWrapper
 * \brief Wrapper for OpenCV Random Trees
 *
 * \ingroup OTBSupervised
 */
class OTBSupervised_EXPORT CvRTreesWrapper
#ifdef OTB_OPENCV_3
  : public cv::ml::RTrees
#else
  : public CvRTrees
#endif
{
public:
  typedef std::vector<unsigned int> VotesVectorType;
  CvRTreesWrapper();
  ~CvRTreesWrapper() override;

  /** Compute the number of votes for each class. */
  void get_votes(const cv::Mat& sample,
                 const cv::Mat& missing,
                 VotesVectorType& vote_count) const;

  /** Predict the confidence of the classifcation by computing the proportion
      of trees which voted for the majority class.
  */
  float predict_confidence(const cv::Mat& sample,
                           const cv::Mat& missing =
                           cv::Mat()) const;

  /** Predict the confidence margin of the classifcation by computing the
      difference in votes between the first and second most voted classes.
      This measure is preferred to the proportion of votes of the majority
      class, since it provides information about the conflict between the
      most likely classes.
  */
  float predict_margin(const cv::Mat& sample,
                          const cv::Mat& missing =
                          cv::Mat()) const;

#ifdef OTB_OPENCV_3

#define OTB_CV_WRAP_PROPERTY(type,name) \
  virtual type get##name() const; \
  virtual void set##name(type val);

#define OTB_CV_WRAP_PROPERTY_REF(type,name) \
  virtual type get##name() const; \
  virtual void set##name(const type &val);

#define OTB_CV_WRAP_CSTREF_GET(type, name) \
  virtual const type& get##name() const;

  // TODO : wrap all method used
  virtual int getVarCount() const;

  virtual bool isTrained() const;

  virtual bool isClassifier() const;

  OTB_CV_WRAP_PROPERTY(int, MaxCategories)
  OTB_CV_WRAP_PROPERTY(int, MaxDepth)
  OTB_CV_WRAP_PROPERTY(int, MinSampleCount)
  OTB_CV_WRAP_PROPERTY(bool, UseSurrogates)
  OTB_CV_WRAP_PROPERTY(int, CVFolds)
  OTB_CV_WRAP_PROPERTY(bool, Use1SERule)
  OTB_CV_WRAP_PROPERTY(bool,TruncatePrunedTree)
  OTB_CV_WRAP_PROPERTY(float, RegressionAccuracy)
  OTB_CV_WRAP_PROPERTY(bool, CalculateVarImportance)
  OTB_CV_WRAP_PROPERTY(int, ActiveVarCount)
  OTB_CV_WRAP_PROPERTY_REF(cv::Mat, Priors)
  OTB_CV_WRAP_PROPERTY_REF(cv::TermCriteria, TermCriteria)
  
  OTB_CV_WRAP_CSTREF_GET(std::vector<int>, Roots)
  OTB_CV_WRAP_CSTREF_GET(std::vector<cv::ml::DTrees::Node>, Nodes)
  OTB_CV_WRAP_CSTREF_GET(std::vector<cv::ml::DTrees::Split>, Splits)
  OTB_CV_WRAP_CSTREF_GET(std::vector<int>, Subsets)

  virtual cv::Mat getVarImportance() const;

  virtual cv::String 	getDefaultName () const;

  virtual void 	read (const cv::FileNode &fn);

  virtual void write (cv::FileStorage &fs) const;

  virtual void 	save (const cv::String &filename) const;

  virtual bool train(cv::InputArray samples, int layout, cv::InputArray responses);

  virtual bool train( const cv::Ptr<cv::ml::TrainData>& trainData, int flags=0 );

  virtual float predict (cv::InputArray samples, cv::OutputArray results=cv::noArray(), int flags=0) const;
  
  static cv::Ptr<CvRTreesWrapper> create();

#undef OTB_CV_WRAP_PROPERTY
#undef OTB_CV_WRAP_PROPERTY_REF
#undef OTB_CV_WRAP_CSTREF_GET

private:
  cv::Ptr<cv::ml::RTrees> m_Impl;
#endif // OTB_OPENCV_3
};

}

#endif
