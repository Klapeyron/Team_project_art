#include "SizeConverter.hpp"

cv::Mat SizeConverter::resizeTemplateIntoSourceImageSize(cv::Mat const& sourceImage, cv::Mat const& image)
{
  cv::Mat output = image;
  const cv::Size size = sourceImage.size();
  cv::resize(image, output, cv::Size(), size.width/xTemplateSize, size.height/yTemplateSize);
  return output;
}
