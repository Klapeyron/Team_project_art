#include "ImageAnalyzer.hpp"
#include <iostream>

std::tuple<bool, Position> ImageAnalyzer::containsImageTemplate(Image const& image, Image const& templ)
{
  Image result;
  cv::Point minLoc; cv::Point maxLoc;
  double minVal, maxVal;

  cv::matchTemplate(image, templ, result, MATCH_METHOD);
  cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
  
  Position position(maxLoc.x, maxLoc.y);
  auto imageMatched = maxVal >= THRESHOLD;
  std::cout << maxVal << std::endl;
  return std::make_tuple(imageMatched, position);
}
