#include "AreaOfInterestCutter.hpp"

cv::Mat AreaOfInterestCutter::cutGreenField(cv::Mat const& tableImage, Position leftUpperPosition)
{
  return tableImage(cv::Rect(leftUpperPosition.getX() + 8, leftUpperPosition.getY() + 6, 726, 597));
}

cv::Mat AreaOfInterestCutter::cutUpperCards(cv::Mat const& greenField)
{
  return greenField(cv::Rect(112, 326, 424, 85));
}

cv::Mat AreaOfInterestCutter::cutLowerCards(cv::Mat const& greenField)
{
  return greenField(cv::Rect(112, 409, 424, 80));
}

cv::Mat AreaOfInterestCutter::cutEnemyCards(cv::Mat const& greenField)
{
  return greenField(cv::Rect(238, 13, 239, 106));
}

cv::Mat AreaOfInterestCutter::cutMiddlePart(cv::Mat const& greenField)
{
  return greenField(cv::Rect(187, 123, 333, 218));
}

cv::Mat AreaOfInterestCutter::cutLeftStackPart(cv::Mat const& greenField)
{
  return greenField(cv::Rect(15, 160, 100, 125));
}

cv::Mat AreaOfInterestCutter::cutRightStackPart(cv::Mat const& greenField)
{
  return greenField(cv::Rect(600, 160, 100, 125));
}
