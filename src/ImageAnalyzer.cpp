#include "ImageAnalyzer.hpp"

bool ImageAnalyzer::containsImageTemplate(Image const& image, Image const& templ)
{
  cv::Mat templateAfterResize, result;
  double minVal, maxVal;
  cv::resize(templ, templateAfterResize, cv::Size(), 0.711458, 0.71111);
      
  cv::matchTemplate(image, templateAfterResize, result, MATCH_METHOD);
  cv::minMaxLoc(result, &minVal, &maxVal);  
  return maxVal >= THRESHOLD;
}
