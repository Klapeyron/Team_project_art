#pragma once

constexpr int xTemplateSize = 1920;
constexpr int yTemplateSize = 1080;

#include <opencv/cv.hpp>

namespace SizeConverter {
cv::Mat resizeTemplateIntoSourceImageSize(cv::Mat const& sourceImage, cv::Mat const& image);
}


