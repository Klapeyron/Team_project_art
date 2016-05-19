#pragma once

#include <opencv/cv.hpp>
#include "Position.hpp"

class AreaOfInterestCutter
{
 public:
  const cv::Rect UpperCardsPosition = cv::Rect(112, 326, 424, 85);
  const cv::Rect LowerCardsPosition = cv::Rect(112, 409, 424, 80);
  const cv::Rect EnemyCardsPosition = cv::Rect(238, 13, 239, 106);
  const cv::Rect MiddlePartPosition = cv::Rect(187, 123, 333, 218);
  const cv::Rect LeftStackPosition = cv::Rect(15, 160, 125, 100);
  const cv::Rect RightStackPosition = cv::Rect(600, 160, 125, 100);
  
  cv::Mat cutGreenField(cv::Mat const& tableImage, Position greenFieldUpperCorner);
  cv::Mat cutUpperCards(cv::Mat const& greenField);
  cv::Mat cutLowerCards(cv::Mat const& greenField);
  cv::Mat cutEnemyCards(cv::Mat const& greenField);
  cv::Mat cutMiddlePart(cv::Mat const& greenField);
  cv::Mat cutLeftStackPart(cv::Mat const& greenField);
  cv::Mat cutRightStackPart(cv::Mat const& greenField);
};
