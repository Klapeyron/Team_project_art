#pragma once

#include <opencv/cv.hpp>

class AreaOfInterestCutter
{
 public:
  // cv::Mat cutGreenField(cv::Mat const& tablecv::Mat);
  cv::Mat cutUpperCards(cv::Mat const& greenField);
  cv::Mat cutLowerCards(cv::Mat const& greenField);
  cv::Mat cutEnemyCards(cv::Mat const& greenField);
  cv::Mat cutMiddlePart(cv::Mat const& greenField);
  cv::Mat cutStackPart(cv::Mat const& greenField);
};
