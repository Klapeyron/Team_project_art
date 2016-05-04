#include "AreaOfInterestCutter.hpp"

Image AreaOfInterestCutter::cutUpperCards(Image const& greenField)
{
  return greenField(cv::Rect(112, 326, 424, 85));
}

Image AreaOfInterestCutter::cutLowerCards(Image const& greenField)
{
  return greenField(cv::Rect(112, 409, 424, 80));
}

Image AreaOfInterestCutter::cutEnemyCards(Image const& greenField)
{
  return greenField(cv::Rect(238, 13, 239, 106));
}

Image AreaOfInterestCutter::cutMiddlePart(Image const& greenField)
{
  return greenField(cv::Rect(187, 123, 333, 218));
}

Image AreaOfInterestCutter::cutStackPart(Image const& greenField)
{
  return greenField(cv::Rect(600, 175, 85, 100));
}
