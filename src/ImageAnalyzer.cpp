#include "ImageAnalyzer.hpp"

std::tuple<bool, Position> ImageAnalyzer::containsImageTemplate(cv::Mat const& image, cv::Mat const& templ)
{
  cv::Mat result;
  cv::Point minLoc; cv::Point maxLoc;
  double minVal, maxVal;

  cv::matchTemplate(image, templ, result, MATCH_METHOD);
  cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

  Position position(maxLoc.x, maxLoc.y);
  auto imageMatched = maxVal >= THRESHOLD;

  return std::make_tuple(imageMatched, position);
}
