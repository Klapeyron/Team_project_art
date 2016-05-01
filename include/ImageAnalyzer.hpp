#pragma once

#include <tuple>
#include <opencv/cv.hpp>
#include "SizeConverter.hpp"
#include "Position.hpp"

const double THRESHOLD = 0.90;
const int MATCH_METHOD = CV_TM_CCOEFF_NORMED;
using Image = cv::Mat;

class ImageAnalyzer {
public:
std::tuple<bool, Position> containsImageTemplate(Image const& image, Image const& templ);
};
