#pragma once

#include <opencv/cv.hpp>
#include "SizeConverter.hpp"

const double THRESHOLD = 0.87;
const int MATCH_METHOD = CV_TM_CCOEFF_NORMED;
using Image = cv::Mat;

class TableAnalyzer {
  bool containsImageTemplate(Image const& image, Image const& templ);
 public:
};
