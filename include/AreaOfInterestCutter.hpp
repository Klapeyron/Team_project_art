#pragma once

#include <opencv/cv.hpp>

using Image = cv::Mat;

class AreaOfInterestCutter
{
 public:
  // Image cutGreenField(Image const& tableImage);
  Image cutUpperCards(Image const& greenField);
  Image cutLowerCards(Image const& greenField);
  Image cutEnemyCards(Image const& greenField);
  Image cutMiddlePart(Image const& greenField);
  Image cutStackPart(Image const& greenField);
};
