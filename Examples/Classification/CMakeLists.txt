#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

project(ClassificationExamples)

add_executable(BayesianPluginClassifier BayesianPluginClassifier.cxx)
target_link_libraries(BayesianPluginClassifier ${OTB_LIBRARIES})

add_executable(ClassificationMapRegularizationExample ClassificationMapRegularizationExample.cxx)
target_link_libraries(ClassificationMapRegularizationExample ${OTB_LIBRARIES})

#OTBDempsterShafer depends OTBMathParser. But OTB_USE_MUPARSER is OFF
if(OTBMathParser_LOADED)
add_executable(DempsterShaferFusionOfClassificationMapsExample DempsterShaferFusionOfClassificationMapsExample.cxx)
target_link_libraries(DempsterShaferFusionOfClassificationMapsExample ${OTB_LIBRARIES})
endif()
add_executable(ExpectationMaximizationMixtureModelEstimator ExpectationMaximizationMixtureModelEstimator.cxx)
target_link_libraries(ExpectationMaximizationMixtureModelEstimator ${OTB_LIBRARIES})

add_executable(KdTreeBasedKMeansClustering KdTreeBasedKMeansClustering.cxx)
target_link_libraries(KdTreeBasedKMeansClustering ${OTB_LIBRARIES})

add_executable(KMeansImageClassificationExample KMeansImageClassificationExample.cxx)
target_link_libraries(KMeansImageClassificationExample ${OTB_LIBRARIES})

add_executable(MajorityVotingFusionOfClassificationMapsExample MajorityVotingFusionOfClassificationMapsExample.cxx)
target_link_libraries(MajorityVotingFusionOfClassificationMapsExample ${OTB_LIBRARIES})

add_executable(ScalarImageKmeansClassifier ScalarImageKmeansClassifier.cxx)
target_link_libraries(ScalarImageKmeansClassifier ${OTB_LIBRARIES})

add_executable(ScalarImageKmeansModelEstimator ScalarImageKmeansModelEstimator.cxx)
target_link_libraries(ScalarImageKmeansModelEstimator ${OTB_LIBRARIES})

add_executable(ScalarImageMarkovRandomField1 ScalarImageMarkovRandomField1.cxx)
target_link_libraries(ScalarImageMarkovRandomField1 ${OTB_LIBRARIES})

add_executable(SOMImageClassificationExample SOMImageClassificationExample.cxx)
target_link_libraries(SOMImageClassificationExample ${OTB_LIBRARIES})

add_executable(SupervisedImageClassificationExample SupervisedImageClassificationExample.cxx)
target_link_libraries(SupervisedImageClassificationExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
