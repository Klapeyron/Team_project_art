#include "TableAnalyzer.hpp"

bool TableAnalyzer::containsImageTemplate(Image const& image, Image const& templ)
{
  cv::Mat templateAfterResize, result;
  double minVal, maxVal;
  cv::resize(templ, templateAfterResize, cv::Size(), 0.711458, 0.711111);
  cv::matchTemplate(image,templ,result,MATCH_METHOD);
  cv::minMaxLoc(result, &minVal, &maxVal);
  return maxVal >= THRESHOLD;
}
